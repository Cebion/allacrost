///////////////////////////////////////////////////////////////////////////////
//            Copyright (C) 2004-2018 by The Allacrost Project
//                         All Rights Reserved
//
// This code is licensed under the GNU GPL version 2. It is free software
// and you may modify it and/or redistribute it under the terms of this license.
// See http://www.gnu.org/copyleft/gpl.html for details.
///////////////////////////////////////////////////////////////////////////////

/** ***************************************************************************
*** \file    map_data.h
*** \author  Tyler Olsen (Roots)
*** \brief   Header file for map data class
***
*** This file contains the data model of the map that the editor manipulates.
*** This is the object that holds all of the data loaded from a map file. The
*** editor view classes request access to pieces of the data managed by this class
*** and edits them appropriately.
*** **************************************************************************/

#pragma once

#include <QString>
#include <QStringList>

#include "editor_utils.h"
#include "tile_context.h"
#include "tile_layer.h"
#include "tileset.h"

namespace hoa_editor {

/** ****************************************************************************
*** \brief Manages all data for an open map file
***
*** This class serves as a protective interface for the modification of any map context or tile
*** layer data. As such, the class is the custodian for all TileLayer and TileContext objects for
*** the currently opened map. The Grid class maintains an instance of this class and calls the
*** appropriate methods in response to events generated by user input. In that sense, this class
*** is the model component in a MV architecture, where Grid serves as the view component.
***
*** The most important roll of this class is to maintain the integrity of the map tile data. For
*** example, when the user adds a new tile layer, this class will make sure that the layer is added
*** to every tile context. When the user requests a context to be removed, the class ensures that
*** there are no other contexts that inherit from the context being removed.
***
*** Many methods on this class return a boolean to indicate success or failure. Because this class is
*** not in any way tied to the user interface, it does not generate any error messages. Instead, it stores
*** the string of the most recent error message. The user interface code should always check for the return status
*** of functions that can produce an error, and upon an error can call GetErrorMessage() to retrieve the error string
*** and decide how to handle it.
*** ***************************************************************************/
class MapData {
public:
	MapData();

	~MapData()
		{ DestroyData(); }

	//! \brief Returns true if any initialized map data is being stored
	bool IsInitialized() const
		{ return (_tile_context_count > 0); }

	//! \name Class member accessor functions
	//@{
	QString GetMapFilename() const
		{ return _map_filename; }

	QString GetMapName() const
		{ return _map_name; }

	uint32 GetMapHeight() const
		{ return _map_height; }

	uint32 GetMapLength() const
		{ return _map_length; }

	bool IsMapModified() const
		{ return _map_modified; }

	std::vector<Tileset*>& GetTilesets()
		{ return _tilesets; }

	uint32 GetTileContextCount() const
		{ return _tile_context_count; }

	uint32 GetTileLayerCount() const
		{ return _tile_layer_count; }

	TileContext* GetSelectedTileContext() const
		{ return _selected_tile_context; }

	TileLayer* GetSelectedTileLayer() const
		{ return _selected_tile_layer; }

	TileLayerProperties* GetSelectedTileLayerProperties() const
		{ return _selected_tile_layer_properties; }

	//! \brief Gets the most recent error message generated by a call and clears that error
	QString GetErrorMessage()
		{ QString error = _error_message; _error_message.clear(); return error; }

	void SetMapFilename(QString filename)
		{ _map_filename = filename; }

	void SetMapName(QString name)
		{ _map_name = name; }

	void SetMapModified(bool value)
		{ _map_modified = value; }
	//@}

	/** \brief Call when creating a new map to initialize the first TileContext object
	*** \param map_length The length of the new map data, in number of tiles
	*** \param map_height The height of the new map data, in number of tiles
	*** \return True only if initialization was successful
	***
	*** If this class currently holds any TileContext data, it will refuse to destroy it and
	*** return false. Call DestroyData() first to safely remove any TileContext data.
	**/
	bool CreateData(uint32 map_length, uint32 map_height);

	/** \brief Call whenever closing an open map to destroy all layers, contexts, and other data
	*** \note Any calls to GetContext or other functions that returned a TileContext or TileLayer
	*** pointer prior to this function being called will have invalid pointers if they were retained.
	*** Make sure to remove any locally stored TileContext object pointers after calling this function.
	**/
	void DestroyData();

	/** \brief Loads all the map contexts from an open map file
	*** \param filename The name of the file where the data should be loaded from
	*** \return True if all data was loaded successfully
	***
	*** The function will return false if it detects that there is already map data loaded. Call DestroyData()
	*** prior to calling this function to avoid this.
	**/
	bool LoadData(QString filename);

	/** \brief Saves all the map context data to the file that the map data was last saved/loaded from
	*** \return True if all data was saved successfully
	*** \note This function will not work if the map data was created with "New..." and not originally loaded from a file
	**/
	bool SaveData()
		{ return SaveData(_map_filename); }

	/** \brief Saves all the map context data to an open map file
	*** \param filename The name of the file where the data should be saved to
	*** \return True if all data was saved successfully
	**/
	bool SaveData(QString filename);

	/** \brief Resizes the map by adding or removing the appropriate number of rows and columns
	*** \param map_length The length of the new map data, in number of tiles
	*** \param map_height The height of the new map data, in number of tiles
	***
	*** New rows and columns are appended to the bottom and right of the map respectively.
	*** Rows and columns removed are also taken from the bottom and right of the map.
	**/
	void ResizeMap(uint32 map_length, uint32 map_height);

	//! \name Tileset Manipulation Methods
	//@{
	Tileset* GetTileset(uint32 tileset_index) const
		{ if (tileset_index < _tilesets.size()) return _tilesets[tileset_index]; else return nullptr; }

	//! \brief Return an ordered list of the definition filenames for each tileset
	QStringList GetTilesetFilenames() const;

	/** \brief Adds a new tileset object to the end of the tileset list
	*** \param new_tileset A pointer to the already created tileset object to add
	*** \return True if the tileset was added successfully
	*** \note The Tileset object passed in this function must have been created via the new operator
	***
	*** Reasons why the tileset add might fail include:
	***   - nullptr pointer
	***   - The tileset object was not already initialized
	***   - The tileset object was already added
	***   - A tileset with the same tileset definition filename already exists
	***
	*** If this method returns true, it assumes responsibility for memory management of the
	*** object and will call delete on all tileset objects when the map data is cleared or the
	*** class is destroyed.
	**/
	bool AddTileset(Tileset* new_tileset);

	/** \brief Removes a tileset from the map and deletes the Tileset object
	*** \param tileset_index The index of the tileset in the tileset list
	**/
	void RemoveTileset(uint32 tileset_index);

	/** \brief Moves a tileset one position up in the tileset list
	*** \param tileset_index The index of the tileset in the tileset list
	**/
	void MoveTilesetUp(uint32 tileset_index);

	/** \brief Moves a tileset one position up in the tileset list
	*** \param tileset_index The index of the tileset in the tileset list
	**/
	void MoveTilesetDown(uint32 tileset_index);
	//@}

	//! \name Tile Layer Manipulation Methods
	//@{
	/** \brief Changes which tile layer is selected for editing on the currently selected map context
	*** \param layer_index The index of the layer to select
	*** \return A pointer to the newly selected TileLayer, or nullptr if the selected layer could not be changed
	**/
	TileLayer* ChangeSelectedTileLayer(uint32 layer_index);

	//! \brief Return an ordered list of names for all tile layers
	QStringList GetTileLayerNames() const;

	//! \brief Returns an ordered list of the properties of each tile layer
	std::vector<TileLayerProperties>& GetTileLayerProperties()
		{ return _tile_layer_properties; }

	TileLayerProperties* GetTileLayerProperties(uint32 layer_index)
		{ if (layer_index >= _tile_layer_count) return nullptr; else return &_tile_layer_properties[layer_index]; }

	/** \brief Makes a tile layer visible in the editor
	*** \param layer_index The index of the layer to show
	**/
	void ShowTileLayer(uint32 layer_index)
		{ if (layer_index <= _tile_layer_count) _tile_layer_properties[layer_index].SetVisible(true); }

	/** \brief Removes visibility of a tile layer in the editor
	*** \param layer_index The index of the layer to hide
	**/
	void HideTileLayer(uint32 layer_index)
		{ if (layer_index <= _tile_layer_count) _tile_layer_properties[layer_index].SetVisible(false); }

	/** \brief Toggles whether or not a tile layer is visible in the editor
	*** \param layer_index The index of the tile layer to toggle visibility for
	**/
	void ToggleTileLayerVisibility(uint32 layer_index);

	/** \brief Activates a tile layer's collision data
	*** \param layer_index The index of the tile layer to activate collisions for
	**/
	void EnableTileLayerCollision(uint32 layer_index)
		{ if (layer_index <= _tile_layer_count) _tile_layer_properties[layer_index].SetCollisionEnabled(true); }

	/** \brief Deactivates a tile layer's collision data
	*** \param layer_index The index of the tile layer to deactivate collisions for
	**/
	void DisableTileLayerCollision(uint32 layer_index)
		{ if (layer_index <= _tile_layer_count) _tile_layer_properties[layer_index].SetCollisionEnabled(false); }

	/** \brief Toggles the activation of a tile layer's collision data
	*** \param layer_index The index of the tile layer to toggle collision data for
	**/
	void ToggleTileLayerCollision(uint32 layer_index);

	/** \brief Adds a new tile layer to all active contexts
	*** \param name The name of the layer to add, as will be see in the editor
	*** \param collision_enabled If true, the tile layer's collision data will be active in the map
	*** \return True if the layer was added successfully
	*** \note The layer name should be unique amongst all existing tile layers
	**/
	bool AddTileLayer(QString name, bool collision_enabled);

	/** \brief Removes a tile later from all active contexts
	*** \param layer_index The index of the layer to remove
	*** \return True if the layer was deleted successfully
	**/
	bool DeleteTileLayer(uint32 layer_index);

	/** \brief Creates a new tile layer by cloning the properties and data of an existing layer
	*** \param layer_index The index of the layer to clone
	*** \return True if the layer was added successfully
	**/
	bool CloneTileLayer(uint32 layer_index);

	/** \brief Renames an existing tile layer
	*** \param layer_index The index of the layer to rename
	*** \param new_name The new name for the tile layer
	*** \return True if the layer was renamed successfully
	*** \note The layer name should be unique amongst all existing tile layers
	**/
	bool RenameTileLayer(uint32 layer_index, QString new_name);

	/** \brief Moves a tile layer one position up in the layer list
	*** \param layer_index The index of the layer that should be moved upward
	*** \return True if the layer was moved successfully
	**/
	bool MoveTileLayerUp(uint32 layer_index)
		{ return SwapTileLayers(layer_index, layer_index - 1); }

	/** \brief Moves a tile layer one position down in the layer list
	*** \param layer_index The index of the layer that should be moved downward
	*** \return True if the layer was moved successfully
	**/
	bool MoveTileLayerDown(uint32 layer_index)
		{ return SwapTileLayers(layer_index, layer_index + 1); }

	/** \brief Swaps the order position of two tile layers
	*** \param index_one Index of the first layer to swap
	*** \param index_two Index of the second layer to swap
	*** \return True if the two layers were swapped successfully
	**/
	bool SwapTileLayers(uint32 index_one, uint32 index_two);

	/** \brief Inserts one or more blank tile rows to all tile layers in each context
	*** \param row_index Indicates the place where the row(s) should be inserted
	*** \param row_count The number of rows to insert (default value: 1)
	*** \note This function does not allow rows to be added on to the bottom of the map. Use ResizeMap() to achieve this instead
	**/
	void InsertTileLayerRows(uint32 row_index, uint32 row_count = 1);

	/** \brief Removes one or more tile rows from all tile layers in each context
	*** \param row_index Indicates the starting location where the row(s) should be removed
	*** \param row_count The number of rows to remove (default value: 1)
	**/
	void RemoveTileLayerRows(uint32 row_index, uint32 row_count = 1);

	/** \brief Inserts one or more blank tile columns to all tile layers in each context
	*** \param row_index Indicates the place where the columns(s) should be inserted
	*** \param row_count The number of columns to insert (default value: 1)
	*** \note This function does not allow columns to be added on to the bottom of the map. Use ResizeMap() to achieve this instead
	**/
	void InsertTileLayerColumns(uint32 row_index, uint32 row_count = 1);

	/** \brief Removes one or more tile columns from all tile layers in each context
	*** \param row_index Indicates the starting location where the column(s) should be removed
	*** \param row_count The number of columns to remove (default value: 1)
	**/
	void RemoveTileLayerColumns(uint32 row_index, uint32 row_count = 1);
	//@}

	//! \name Tile Context Manipulation Methods
	//@{
	/** \brief Changes which tile context is selected for editing
	*** \param context_id The ID of the context to select
	*** \return A pointer to the newly selected TileContext, or nullptr if the context did not exist
	*** \note This function also changes the selected tile layer to point to the corresponding layer in the newly selected context
	**/
	TileContext* ChangeSelectedTileContext(int32 context_id);

	//! \brief Returns the ordered list of names for all tile contexts
	QStringList GetTileContextNames() const;

	/** \brief Returns an ordered list of all names of the contexts that each context inherits from
	*** \note Contexts which do not inherit from another context will be represented with an empty string
	**/
	QStringList GetInheritedTileContextNames() const;

	/** \brief Creates a new TileContext object and add it to the end of the context list
	*** \param name The name to assign to the context (must be a non-empty string)
	*** \param inheriting_context_id The ID of the context that this context should inherit from.
	*** Leaving this argument empty will use a default value designating the context as a base context
	*** \return A pointer to the newly created TileContext, or nullptr if an error prevented the context from being created
	***
	*** Possible errors that could prevent context creation include exceeding the maximum number of
	*** contexts allowed (MAX_CONTEXTS), an existing context with the same name, or an invalid context ID argument.
	**/
	TileContext* AddTileContext(QString name, int32 inheriting_context_id = INVALID_CONTEXT);

	/** \brief Deletes an existing TileContext object
	*** \param context_id The ID of the context to delete
	*** \return True only if the context was deleted successfully
	***
	*** A context may fail to be deleted if it's the final base context in the context list or one or more
	*** contexts inherit from the context.
	**/
	bool DeleteTileContext(int32 context_id);

	/** \brief Creates a new TileContext object by copying the data and properties of an existing TileContext
	*** \param context_id The ID of the context to clone
	*** \return A pointer to the newly created TileContext, or nullptr if an error prevented the context from being cloned
	**/
	TileContext* CloneTileContext(int32 context_id);

	/** \brief Renames an existing TileContext object
	*** \param context_id The ID of the context to rename
	*** \param new_name The name to set for the context
	*** \return True if the context was renamed successfully
	***
	*** The name should be unique among all existing TileContext names. Note that any previous calls to
	*** GetTileContextNames() or GetInheritedTileContextNames() that retained the QStringList from those
	*** calls will be outdated if this function completes successfully. You should always remember to
	*** update any external context name lists after a rename operation.
	**/
	bool RenameTileContext(int32 context_index, QString new_name);

	/** \brief Changes the inheritance property of a tile context
	*** \param context_id The ID of the context to change
	*** \param inherit_id The ID of the context which should be inherited from (use INVALID_CONTEXT to remove inheritance)
	*** \return True if the inheritance was changed successfully
	**/
	bool ChangeInheritanceTileContext(int32 context_id, int32 inherit_id);

	/** \brief Removes the inheritance property of a tile context
	*** \param context_id The ID of the context to remove the inheritance from
	*** \return True if the inheritance was removed successfully
	**/
	bool RemoveInheritanceTileContext(int32 context_id)
		{ return ChangeInheritanceTileContext(context_id, INVALID_CONTEXT); }

	/** \brief Moves a context up in the list
	*** \param context_id The ID of the context to move
	*** \return True if the move operation was successful. Will fail if the context is already at the top of the list
	**/
	bool MoveTileContextUp(int32 context_id)
		{ return SwapTileContexts(context_id, context_id - 1); }

	/** \brief Moves a context down in the list
	*** \param context_id The ID of the context to move
	*** \return True if the move operation was successful. Will fail if the context is already at the bottom of the list
	**/
	bool MoveTileContextDown(int32 context_id)
		{ return SwapTileContexts(context_id, context_id + 1); }

	/** \brief Swaps the order position of two tile contexts
	*** \param first_id ID of the first context to swap
	*** \param second_id ID of the second context to swap
	*** \return True if the two contexts were swapped successfully
	**/
	bool SwapTileContexts(int32 first_id, int32 second_id);

	/** \brief Returns a pointer to a TileContext with a specified id
	*** \param context_id The ID of the context to retrieve
	*** \return A pointer to the TileContext, or nullptr if no context with the given ID was found
	**/
	TileContext* FindTileContextByID(int32 context_id) const;

	/** \brief Returns a pointer to a TileContext with a specified name
	*** \param context_name The name of the context to retrieve
	*** \return A pointer to the TileContext, or nullptr if no context with the given name was found
	*** \note Context names are guaranteed to be unique, so a name will never map to more than one context
	**/
	TileContext* FindTileContextByName(QString context_name) const;

	/** \brief Returns a pointer to a TileContext at the given index in the context list
	*** \param context_index The index of the context to retrieve that should map to the _all_map_contexts container
	*** \return A pointer to the TileContext, or nullptr if no context with the given name was found
	*** \note Context names are guaranteed to be unique, so a name will never map to more than one context
	**/
	TileContext* FindTileContextByIndex(uint32 context_index) const;
	//@}

private:
	/** \brief The file name where the map data was most recently loaded from or saved to
	*** \note Will be an empty string if this is a new map that has not been saved to a file
	**/
	QString _map_filename;

	//! \brief The name of the map as seen in the editor
 	QString _map_name;

	//! \brief Comma-delimited string of the names of the designers who created the map
	QString _map_designers;

	//! \brief A user-provided description of the map
	QString _map_description;

	//! \brief The length of the map in number of tiles
	uint32 _map_length;

	//! \brief The height of the map in number of tiles
	uint32 _map_height;

	//! \brief True whenever the map contains modified data that has not been saved
	bool _map_modified;

	//! \brief The number of tile layers that the map contains
	uint32 _tile_layer_count;

	//! \brief The number of map (tile) contexts that the map contains
	uint32 _tile_context_count;

	//! \brief A pointer to the map context currently selected by the user
	TileContext* _selected_tile_context;

	/** \brief A pointer to the tile layer currently selected by the user
	*** \note This tile layer exists in the _active_tile_context object
	**/
	TileLayer* _selected_tile_layer;

	//! \brief A pointer to the map context currently selected by the user
	TileLayerProperties* _selected_tile_layer_properties;

	//! \brief Holds the collision data computed from each context and tile layer
	std::vector<std::vector<uint32> > _collision_data;

	//! \brief Stores all tilesets used by the map
	std::vector<Tileset*> _tilesets;

	/** \brief Stores all TileContext objects for the given map
	***
	*** This container always has a size of MAX_CONTEXTS. The value at index 0 is always non-nullptr
	*** (except when no map is loaded) while other locations may or may not be nullptr depending on
	*** the number of contexts that have been created. All non-nullptr entries are always contained
	*** within the front of the container, so you wouldn't have a situation where you'd have nullptr
	*** values inbetween valid context objects. The context at index i will always have an ID value of i+1.
	**/
	std::vector<TileContext*> _all_tile_contexts;

	//! \brief An ordered container of the shared properties for each tile layer across all contexts
	std::vector<TileLayerProperties> _tile_layer_properties;

	/** \brief A tile layer that contains nothing but empty tiles, used for TileContext construction
	***
	*** This structure is maintained to the current height and length of the open map so that when a new
	*** context is created or tile layer is added, this member can be used to create a new empty layer
	*** of the correct size.
	**/
	TileLayer _empty_tile_layer;

	//! \brief Contains the error message generated by the most recently called method that failed
	QString _error_message;

	//! \brief Returns the index where the context object is stored
	uint32 _GetTileContextIndex(TileContext* context)
		{ return static_cast<uint32>(context->GetContextID() - 1); }

	/** \brief Given a string, creates a modified version of the string that appends text to indicate that it is a clone
	*** \param name A reference to the string containing the name to clone
	*** \param taken_names A reference to a list of names that the return value can not be
	*** \return The name of the cloned string
	***
	*** This function will first try to append " (Clone)". If that name is taken, it will try the name (Clone #1). The number
	*** value will continue to increment until it finds a name that is not in the taken_names list.
	**/
	QString _CreateCloneName(const QString& name, const QStringList& taken_names) const;

	/** \brief Computes the collision grid from the current map data and saves the result to _collision_data
	***
	*** The collision grid is four times the size of the tile grid (twice as long, and twice as high).
	*** The tileset data contains the collision information for every quadrant of its map tiles. The
	*** data is computed by looking at the collsion data for each tile in every position of the map
	*** grid on every layer that has the layer collision property enabled. This is done for each tile
	*** context, and the results are bitmasked together so that the collision data for all potential
	*** 32 contexts can fit within a single 32-bit integer.
	***
	**/
	void _ComputeCollisionData();
}; // class MapData

} // namespace hoa_editor