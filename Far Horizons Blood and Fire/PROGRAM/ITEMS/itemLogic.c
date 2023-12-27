#include "items\items.h"

object randItemModels[MAX_LOADED_RANDITEMS];
object itemModels[ITEMS_QUANTITY];
object buttonModels[MAX_BUTTONS];

aref useLocators[MAX_BUTTONS];
aref useAttrs[MAX_BUTTONS];
int  useLocatorsCount=0;

int iScriptItemStartNum=0;
int iScriptItemEndNum=0;
int iScriptItemCount=0;

float SPAWN_TIME = 168.0; //hours = 168.0

void Items_LoadModel (ref _itemModel, ref _item)
{
	if (!IsEntity(_itemModel))
	{
		string itemFolder = "items";
		CreateEntity(&_itemModel, "MODELR");
		if (CheckAttribute(_item, "folder")) itemFolder = _item.folder;
		SendMessage(&_itemModel, "ls", MSG_MODEL_SET_DIRPATH, itemFolder+"\");
		SendMessage(&_itemModel, "ls", MSG_MODEL_LOAD_GEO, itemFolder + "\" + _item.model);
		SendMessage(&_itemModel, "ls", MSG_MODEL_SET_DIRPATH,"");
		SendMessage(&_itemModel, "ls", MSG_MODEL_SET_TECHNIQUE, "RandItem");
		LayerAddObject(EXECUTE, &_itemModel, ITEMS_LAYER);
		LayerAddObject(REALIZE, &_itemModel, ITEMS_LAYER);
	}
}

void Item_OnLoadLocation(string currentLocation)
{
	// precache active locators in current location
	aref   activeLocation;
	ref chr = GetMainCharacter();
	int currentLocationIndex = FindLocation(currentlocation);
	chr.itemLocationIndex = currentLocationIndex;
	aref aloc, an, al;
	useLocatorsCount = 0;
	makearef(activeLocation, Locations[currentLocationIndex]);
	makearef (aloc, activeLocation.items);
	for (int i=0; i<GetAttributesNum(aloc); i++)
	{
		an = GetAttributeN(aloc, i);
		if (!CheckAttribute(an,"locator"))
			continue;
		if (!FindLocator(activeLocation.id, an.locator, &al, true))
			continue;

		useLocators[useLocatorsCount] = al;
		useAttrs[useLocatorsCount] = an;
		useLocatorsCount++;
	}
	
	// load buttons & items
	Items_ShowButtons(currentLocation);
	RandItems_OnLoadLocation(activeLocation);
	Box_OnLoadLocation(activeLocation);

	for (int itemN=0; itemN<ITEMS_QUANTITY; itemN++)
	{
		if (!CheckAttribute(Items[itemN], "startLocator"))
			continue;
		if (!CheckAttribute(Items[itemN], "startLocation"))
			continue;
		if (!CheckAttribute(Items[itemN], "shown"))
			continue;
		if (Items[itemN].shown == "1")
		{
		if (Items[itemN].startLocation == currentLocation)
		{
			Items_ShowItem(itemN);
		}}
	}
}

void Item_OnUnLoadLocation()
{
	//Trace("ItemLogic: On unload location");
	for (int itemN=0; itemN<ITEMS_QUANTITY; itemN++)
	{
		if (IsEntity(&itemModels[itemN]))
			DeleteClass(&itemModels[itemN]);
	}
	for (int buttonN=0; buttonN<MAX_BUTTONS; buttonN++)
	{
		if (IsEntity(&buttonModels[buttonN]))
			DeleteClass(&itemModels[buttonN]);
	}

	RandItems_OnUnloadLocation();

	ref chr = GetMainCharacter();
	chr.itemLocationIndex = -1;
}

void Item_OnEnterLocator(aref _location, string _locator)
{
	//Trace("ItemLogic: Entered locator "+_locator);
	ref chr = GetMainCharacter();
	int itemN;

	if (findsubstr(_locator, "button", 0) != -1)
	{ // use item
        for (itemN=ItemsForLocators_start; itemN<ItemsForLocators_end; itemN++)
		{
			if (!CheckAttribute(Items[itemN], "useLocator"))
				continue;
			if (!CheckAttribute(Items[itemN], "useLocation"))
				continue;
			if (!CheckAttribute(Items[itemN], "shown"))
				continue;
			if (Items[itemN].shown == "0")
			{
			if (Items[itemN].useLocation == _location.id)
			{
			if (Items[itemN].useLocator == _locator)
			{
				Log_SetActiveAction("Action");
				chr.activeItem = itemN;
				chr.activeLocator = _locator;
				chr.activeRandItem = false;
				chr.activeItem.pick = false;
			}}}
		}
	}
	else
	{
		for (itemN=ItemsForLocators_start; itemN<ItemsForLocators_end; itemN++)
		{
			if (!CheckAttribute(Items[itemN], "startLocator"))
				continue;
			if (!CheckAttribute(Items[itemN], "startLocation"))
				continue;
			if (!CheckAttribute(Items[itemN], "shown"))
				continue;
			if (Items[itemN].startLocator == _locator)
			{
			if (Items[itemN].shown == "1")
			{
			if (Items[itemN].startLocation == _location.id)
			{
				Log_SetActiveAction("Pick");
				chr.activeItem = itemN;
				chr.activeLocator = _locator;
				chr.activeRandItem = false;
				chr.activeItem.pick = true;
			}}}
		}
	}
    // ===> eddy. проверка вхождения ГГ в локаторы группы Item.
    QuestCheckEnterLocItem(_location, _locator);
    // <===
	BLI_RefreshCommandMenu();
}

void Item_OnExitLocator(aref _location, string _locator)
{
	Log_SetActiveAction("Nothing");
	Characters[GetMainCharacterIndex()].activeItem = "";
	BLI_RefreshCommandMenu();
    // ===> eddy. проверка выхождения ГГ из локаторов группы Item.
    QuestCheckExitLocItem(_location, _locator);
    // <===
}

void Item_OnPickItem()
{
	Log_SetActiveAction("Nothing");
	aref activeLocation;
	ref chr = GetMainCharacter();
	makearef(activeLocation, Locations[sti(chr.itemLocationIndex)]);

	int langFile = LanguageOpenFile("ItemsDescribe.txt");
	string displayItemName, youvegotString;
	youvegotString = LanguageConvertString(langFile, "youve_got");
	PlayStereoSound("interface\important_item.wav");

	if (chr.activeRandItem == true)
	{
		string activeRandItemAttribute = "RandItemType"+sti(chr.activeItem);
		if (CheckAttribute(activeLocation, activeRandItemAttribute))
		{
			SendMessage(&randItemModels[sti(chr.activeItem)], "lslff", MSG_MODEL_BLEND, "blenditemlit", 1000, 1.0, 0.0);
			GenerateAndAddItems(GetMainCharacter(), Items[sti(activeLocation.(activeRandItemAttribute))].id, 1);
			displayItemName = LanguageConvertString(langFile, Items[sti(activeLocation.(activeRandItemAttribute))].name);
			Log_SetStringToLog(youvegotString+" "+displayItemName+"!");

			activeLocation.(activeRandItemAttribute) = -1;
		}
	}
	else
	{
		int activeItem = sti(chr.activeItem);
		//Trace("ItemLogic: picking item "+Items[activeItem].id);
		Items[activeItem].shown = false;
		SendMessage(&itemModels[activeItem], "lslff", MSG_MODEL_BLEND, "blenditemlit", 1000, 1.0, 0.0);
		GenerateAndAddItems(GetMainCharacter(), Items[activeItem].id, 1);

		displayItemName = LanguageConvertString(langFile, Items[activeItem].name);
		Log_SetStringToLog(youvegotString+" "+displayItemName+"!");
		// ===> перехват взятия предметов из локатора item на метод обрабоки для квестовых нужд 
		QuestCheckTakeItem(activeLocation, Items[activeItem].id);
		// <===
	}
	DeleteAttribute(chr,"activeItem");
	LanguageCloseFile(langFile);
}

void Item_OnUseItem()
{
	aref aloc, an, al;
	aref activeLocation;
	ref chr = GetMainCharacter();
	int activeItem = sti(chr.activeItem);
	makearef(activeLocation, Locations[sti(chr.itemLocationIndex)]);

	makearef (aloc, activeLocation.items);
	// boal баг! нет предмета, а он работает!!!
    if (!CheckCharacterItem(chr, Items[activeItem].id))
    {
		Log_SetStringToLog(XI_ConvertString("You have not need item"));
        PlaySound("knock");
        return;
    }
    Log_SetActiveAction("Nothing");
	//Log_SetStringToLog("Using "+Items[activeItem].id);
	//Trace("ItemLogic: using item "+Items[activeItem].id);
	Items[activeItem].shown = true;
	Items[activeItem].shown.used = true; //использован
	Items_LoadModel(&itemModels[activeItem], &Items[activeItem]);

	FindLocator(activeLocation.id, chr.activeLocator, &al, true);
	SendMessage(&itemModels[activeItem], "lffffffffffff", MSG_MODEL_SET_POSITION, makeFloat(al.x), makeFloat(al.y), makeFloat(al.z), makeFloat(al.vx.x), makeFloat(al.vx.y), -makeFloat(al.vx.z), makeFloat(al.vy.x), makeFloat(al.vy.y), -makeFloat(al.vy.z), makeFloat(al.vz.x), makeFloat(al.vz.y), -makeFloat(al.vz.z));
	SendMessage(&itemModels[activeItem], "lslff", MSG_MODEL_BLEND, "blenditem", 1000, 0.0, 1.0);

	SetEventHandler("frame", "Item_OnUseFrame", 0);
	TakeItemFromCharacter(GetMainCharacter(), Items[activeItem].id);
	Items[activeItem].startLocator = "";
	al.active = true;
	al.timePassed = 0;

	int langFile = LanguageOpenFile("ItemsDescribe.txt");
	string displayItemName, youvegotString;
	youvegotString = LanguageConvertString(langFile, "used_item");
	displayItemName = LanguageConvertString(langFile, Items[activeItem].name);
	Log_SetStringToLog(youvegotString+" "+displayItemName+"!");
    PlaySound("EventMap");

	DeleteAttribute(chr,"activeItem");
	
	CompleteQuestName("OnUse_"+Items[activeItem].id, "");
}

void Item_OnUseFrame()
{
	aref aloc, an, al;
	aref activeLocation;
	ref chr = GetMainCharacter();

    if (CheckAttribute(chr, "itemLocationIndex") && sti(chr.itemLocationIndex) > 0) // boal fix 230804
    {
    	makearef(activeLocation, Locations[sti(chr.itemLocationIndex)]);

    	makearef (aloc, activeLocation.items);
    	bool usedOnFrame=false;
    	for (int i=0; i<useLocatorsCount; i++)
    	{
    		an = useAttrs[i];
    		al = useLocators[i];
    		if (!CheckAttribute(al, "active"))
    			continue;
    		if (al.active != "1")
    			continue;
    		int timePassed = sti(al.timePassed) + GetDeltaTime();
    		al.timePassed = timePassed;
    		if (timePassed > BUTTON_ACTIVATION_TIME)
    		{
    			al.active = false;
    			continue;
    		}

    		usedOnFrame = true;
    		float timeK = makefloat(timePassed) / makefloat(BUTTON_ACTIVATION_TIME);
    		float deltaY = makefloat(an.deltaY) * timeK;

    		SendMessage(&buttonModels[i], "lffffffffffff", MSG_MODEL_SET_POSITION, makeFloat(al.x), makeFloat(al.y)+deltaY, makeFloat(al.z), makeFloat(al.vx.x), makeFloat(al.vx.y), -makeFloat(al.vx.z), makeFloat(al.vy.x), makeFloat(al.vy.y), -makeFloat(al.vy.z), makeFloat(al.vz.x), makeFloat(al.vz.y), -makeFloat(al.vz.z));

    		for (int j=0; j<ITEMS_QUANTITY; j++)
    		{
    			if (Items[j].useLocator == an.locator)
    			{
    				SendMessage(&itemModels[j], "lffffffffffff", MSG_MODEL_SET_POSITION, makeFloat(al.x), makeFloat(al.y)+makeFloat(an.itemDeltaY)+deltaY, makeFloat(al.z), makeFloat(al.vx.x), makeFloat(al.vx.y), -makeFloat(al.vx.z), makeFloat(al.vy.x), makeFloat(al.vy.y), -makeFloat(al.vy.z), makeFloat(al.vz.x), makeFloat(al.vz.y), -makeFloat(al.vz.z));
    				break;
    			}
    		}
    	}
    }
    else
    {
        if (MOD_BETTATESTMODE == "On") Log_Info("Error: Item_OnUseFrame нет mch.itemLocationIndex ");
    }
	if (!usedOnFrame)
		DelEventHandler("frame", "Item_OnUseFrame");
}

void Items_ShowButtons(string _locationName)
{
	aref al;
	//Trace("ItemLogic: found "+useLocatorsCount+" buttons");
	for (int i=0; i<useLocatorsCount; i++)
	{
		Items_LoadModel(&buttonModels[i], &useAttrs[i]);
		al = useLocators[i];
		if (useAttrs[i].trigged != "1")
			SendMessage(&buttonModels[i], "lffffffffffff", MSG_MODEL_SET_POSITION, makeFloat(al.x), makeFloat(al.y), makeFloat(al.z), makeFloat(al.vx.x), makeFloat(al.vx.y), -makeFloat(al.vx.z), makeFloat(al.vy.x), makeFloat(al.vy.y), -makeFloat(al.vy.z), makeFloat(al.vz.x), makeFloat(al.vz.y), -makeFloat(al.vz.z));
		else
			SendMessage(&buttonModels[i], "lffffffffffff", MSG_MODEL_SET_POSITION, makeFloat(al.x), makeFloat(al.y)+makefloat(useAttrs[i].deltaY), makeFloat(al.z), makeFloat(al.vx.x), makeFloat(al.vx.y), -makeFloat(al.vx.z), makeFloat(al.vy.x), makeFloat(al.vy.y), -makeFloat(al.vy.z), makeFloat(al.vz.x), makeFloat(al.vz.y), -makeFloat(al.vz.z));
	}
}

void Items_ShowItem(int _itemN)
{
	aref al;
	aref activeLocation;
	ref chr = GetMainCharacter();
	makearef(activeLocation, Locations[sti(chr.itemLocationIndex)]);

	if (Items[_itemN].model == "")
	{
		Trace("ItemLogic: no model for item "+activeLocation.id+"."+Items[_itemN].id);
		return;
	}

	if (Items[_itemN].startLocator != "")
	{ //unused
		Items_LoadModel(&itemModels[_itemN], &Items[_itemN]);

		if (!FindLocator(Items[_itemN].startLocation, Items[_itemN].startLocator, &al, true))
			Trace("ItemLogic: locator for item "+_itemN+" not found! ["+Items[_itemN].startLocation+">>"+Items[_itemN].startLocator);
		Trace ("ItemLogic: showing item at "+al.x+", "+al.y+", "+al.z);
		SendMessage(&itemModels[_itemN], "lffffffffffff", MSG_MODEL_SET_POSITION, makeFloat(al.x), makeFloat(al.y), makeFloat(al.z), makeFloat(al.vx.x), makeFloat(al.vx.y), -makeFloat(al.vx.z), makeFloat(al.vy.x), makeFloat(al.vy.y), -makeFloat(al.vy.z), makeFloat(al.vz.x), makeFloat(al.vz.y), -makeFloat(al.vz.z));
	}
	else
	{ //used
		if (Items[_itemN].useLocation != activeLocation.id)
			return;

		Items_LoadModel(&itemModels[_itemN], &Items[_itemN]);

		for (int i=0; i<useLocatorsCount; i++)
		{
			al = useLocators[i];
			if (useAttrs[i].locator == Items[_itemN].useLocator)
			{
				SendMessage(&itemModels[i], "lffffffffffff", MSG_MODEL_SET_POSITION, makeFloat(al.x), makeFloat(al.y)+makeFloat(useAttrs[i].itemDeltaY)+makeFloat(useAttrs[i].deltaY), makeFloat(al.z), makeFloat(al.vx.x), makeFloat(al.vx.y), -makeFloat(al.vx.z), makeFloat(al.vy.x), makeFloat(al.vy.y), -makeFloat(al.vy.z), makeFloat(al.vz.x), makeFloat(al.vz.y), -makeFloat(al.vz.z));
				return;
			}
		}
	}
}

void Items_HideItem(int itemN)
{
}

int Items_FindItem(string itemID, ref itemARef)
{
	aref curItem;
	
	for(int i = 0; i < TOTAL_ITEMS; i++)
	{
		makearef(curItem,Items[i]);
		
		if(CheckAttribute(curItem, "ID") && curItem.id == itemID)
		{
			itemARef = curItem;
			return i;
		}
	}
	
	return -1;
}

int Items_FindItemIdx(string itemID) // нужно для поиска только номера
{
	return GetItemIndex(itemID);
}

// ************** RANDOM ITEMS *****************
void RandItems_OnLoadLocation(ref _location)
{
	string locatorName;
	aref randItemLocator;
	for (int i=1; i<MAX_LOADED_RANDITEMS; i++)
	{
		string lastSpawnTimeString = "LastSpawnTime"+i;
		string randItemTypeString = "RandItemType"+i;
		locatorName = "randitem"+i;
		if (!FindLocator(_location.id, locatorName, &randItemlocator, true)) break;

        // boal -->
        if (!SetLocationQuestRandItem(i, _location, locatorName, randItemLocator))
        {
        // boal <--
            // randitem locator found
    		if (CheckAttribute(_location, lastSpawnTimeString))
    		{ // already spawned?
    			if (CheckAttribute(_location, randItemTypeString))
    			{
    				if (sti(_location.(randItemTypeString)) != -1)
    				{
    					RandItems_DrawRandItem(i, _location, randItemLocator);
    				}
    				else
    				{ //already picked up, try to respawn
    					if ((stf(_location.(lastSpawnTimeString))+SPAWN_TIME) < Items_MakeTime(GetTime(), GetDataDay(), GetDataMonth(), GetDataYear()))
    					{
    						RandItems_SpawnRandItem(i, _location, randItemLocator);
    					}
    				}
    			}
    			else
    			{ // nothing here
    				if ((stf(_location.(lastSpawnTimeString))+SPAWN_TIME) < Items_MakeTime(GetTime(), GetDataDay(), GetDataMonth(), GetDataYear()))
    				{
    					RandItems_SpawnRandItem(i, _location, randItemLocator);
    				}
    			}
    		}
    		else
    		{
    			RandItems_SpawnRandItem(i, _location, randItemLocator);
    		}
		}
	}
}

void RandItems_OnUnloadLocation()
{
	for (int itemN=0; itemN<MAX_LOADED_RANDITEMS; itemN++)
	{
		if (IsEntity(&randItemModels[itemN])) DeleteClass(&randItemModels[itemN]);
	}
}

float Items_MakeTime (float _h, float _d, float _m, float _y)
{
	return (_h+24.0*(_d + 30.0*(_m + 12.0*_y)));
}

void RandItems_SpawnRandItem(int _index, aref _location, aref al)
{
	aref randItem;
	string lastSpawnTimeString;
	string sRandItem;
	int n;

	string alName = GetAttributeName(al);
	string alNameAttribute = "items."+alName;
	if (CheckAttribute(_location, alNameAttribute))
	{
		n = Items_FindItem(_location.items.(alName), &randItem);
		if (n == -1) return;

		lastSpawnTimeString = "LastSpawnTime"+_index;
		_location.(lastSpawnTimeString) = Items_MakeTime(0, 15, 1, 2003);
	}
	else
	{
		sRandItem = SelectRandItem(_location.type, "Mein");
	    Items_FindItem(sRandItem, &randItem);
		n = randItem;
		if(n == -1) return;

		lastSpawnTimeString = "LastSpawnTime"+_index;
		_location.(lastSpawnTimeString) = Items_MakeTime(GetTime(), GetDataDay(), GetDataMonth(), GetDataYear());
	}
	
    if (GetCharacterSkillToOld(GetMainCharacter(), SKILL_FORTUNE) < rand(15)) return; // boal fix
    
	if (!CheckAttribute(randItem, "model") || randItem.model == "")
	{
		Trace("ItemLogic: no model for item "+_location.id+"."+randItem.id);
		return;
	}

	Items_LoadModel(&randItemModels[_index],  randItem);
	SendMessage(&randItemModels[_index], "lffffffffffff", MSG_MODEL_SET_POSITION, makeFloat(al.x), makeFloat(al.y), makeFloat(al.z), makeFloat(al.vx.x), makeFloat(al.vx.y), -makeFloat(al.vx.z), makeFloat(al.vy.x), makeFloat(al.vy.y), -makeFloat(al.vy.z), makeFloat(al.vz.x), makeFloat(al.vz.y), -makeFloat(al.vz.z));

	lastSpawnTimeString = "RandItemType"+_index;
	_location.(lastSpawnTimeString) = n;
}

void RandItems_DrawRandItem(int _index, aref _location, aref al)
{
	string randItemTypeString = "RandItemType"+_index;
	int n = sti(_location.(randItemTypeString));
	
    if (GetCharacterSkillToOld(GetMainCharacter(), SKILL_FORTUNE) < rand(15)) return; // boal fix
    
	if (Items[n].model == "")
	{
		Trace("ItemLogic: no model for item "+_location.id+"."+Items[n].id);
		return;
	}
	
	Items_LoadModel(&randItemModels[_index],  &Items[n]);
	SendMessage(&randItemModels[_index], "lffffffffffff", MSG_MODEL_SET_POSITION, makeFloat(al.x), makeFloat(al.y), makeFloat(al.z), makeFloat(al.vx.x), makeFloat(al.vx.y), -makeFloat(al.vx.z), makeFloat(al.vy.x), makeFloat(al.vy.y), -makeFloat(al.vy.z), makeFloat(al.vz.x), makeFloat(al.vz.y), -makeFloat(al.vz.z));
}

void RandItem_OnEnterLocator(aref _location, string _locator)
{

    int randIndex = sti(strcut(_locator, 8, strlen(_locator)-1));
	string randItemAttribute = "RandItemType"+randIndex;
	ref chr = GetMainCharacter();
	if (CheckAttribute(_location, randItemAttribute))
	{
		int currentType = sti(_location.(RandItemAttribute));
		if (currentType != -1)
		{
			Log_SetActiveAction("Pick");
			chr.activeItem = randIndex;
			chr.activeLocator = _locator;
			chr.activeRandItem = true;
			chr.activeItem.pick = true;
			BLI_RefreshCommandMenu();
		}
	}
}

// ****************** BOXES ********************
void Box_EnterToLocator(aref loc, string locName)
{
	if(!CheckAttribute(loc, locName)) return;
	
	aref boxLocator;
	
	if(!CheckAttribute(loc, locName + ".check"))
	{
	    if(rand(4) == 1) // Экку Korsar - на лету генерим рандомайзер сундуков
	    {
			if(!CheckAttribute(loc, locName+".key") && !CheckAttribute(loc, locName+".closed") && !CheckAttribute(loc, locName+".QuestClosed") && !CheckAttribute(loc, locName+".QuestOpened") && !CheckAttribute(loc, "boarding") && !HasSubStr(loc.id, "My_") && !HasSubStr(locName, "private"))
			{
		        if(rand(1) == 1)
		    	{
	    			Log_Testinfo("Закрытый сундук");
	       			loc.(locName).closed = true;
	    		}
    	    	else
		    	{
	    			Log_Testinfo("Закрытый на простой ключ сундук");
		   			loc.(locName).key = "Key1";
			    }
		
		       	makearef(boxLocator, loc.(locName));
	        	FillClosedChestForItems(loc, boxLocator);
		
	        	loc.(locName).GenClosed = true;
			}
			else
			{
				Log_Testinfo("Сундук не тронут рандомом");
			}
	    }
		loc.(locName).check = true;
	}
	
	
	if(CheckAttribute(loc, locName+".QuestOpened"))
    {
		Log_Testinfo("Сундук принудительно открыт");
	    DeleteAttribute(loc, locName+".Closed");
	    DeleteAttribute(loc, locName+".key");
	}
	
	//if(HasSubStr(locName, "box"))
	//{
		// check if box opened
		if(!CheckAttribute(loc, locName+".opened"))
	    {
			if(CheckAttribute(loc, locName+".key"))
			{
				if(CheckCharacterItem(pchar, loc.(locName).key)) //проверяем ключ
				{
					loc.(locName).opened = true;
					PlaySound("interface\key.wav");
					
					if(CheckAttribute(loc, locName+".key.delItem"))
					{
						TakeItemFromCharacter(pchar, loc.(locName).key); //забрать ключ
					}
				}
				else 
				{
					Log_SetStringToLog(XI_ConvertString("You have not need key"));
					PlaySound("interface\box_locked.wav");
					//return;
			    }
			}
			else
			{	
				if(CheckAttribute(loc, locName+".closed")) //проверяем, не закрыт ли сундук
				{
					Log_SetStringToLog(XI_ConvertString("Box is closed"));
					PlaySound("interface\door_locked.wav");
					//return;
			    }
		    }
	    }
	//}
	
	// Warship 15.08.09 Проверка на квестовую закрытость сундуков (не только приватов, вообще всех)
	if(CheckAttribute(loc, locName + ".QuestClosed"))
	{
		Log_SetStringToLog(XI_ConvertString("Box is closed"));
		PlaySound("interface\door_locked.wav");
		return;
	}
	
	if(CheckAttribute(loc, locName + ".NotChest") && !CheckAttribute(loc, locName + ".MustBeOpened")) // Jason: для выброшенных на берег сундуков //Экку Korsar - чек на доп. аттрибут, если надо по квесту открыть.
	{
		Log_Testinfo("Нет доступа");
		return;
	}
	
	pchar.boxname = locName;
	
	bool bHack = CheckAttribute(loc, locName+".key") && loc.(locName).key == "Key1" && !CheckAttribute(loc, locName+".opened");
	
	if(CheckAttribute(loc, locName+".closed") || bHack)
	{
		pchar.boxname.closed = true;
		if(CheckCharacterPerk(pchar, "Safecracker"))
		{
        	Log_SetActiveAction("HackBox");
	    	Log_Testinfo("Ломаем");
		}
    }
	else
    {
    	Log_SetActiveAction("OpenBox");
		Log_Testinfo("Открываем");
	}
    BLI_RefreshCommandMenu();
}

void Box_ExitFromLocator(aref loc, string locName)
{
	DeleteAttribute(GetMainCharacter(),"boxname");
	Log_SetActiveAction("Nothing");
	BLI_RefreshCommandMenu();
}

void OpenBoxProcedure()
{
	ref chr = GetMainCharacter();
	if( !CheckAttribute(chr,"boxname") ) return;
	int locidx = FindLoadedLocation();
	if(locidx<0) return;
	string atrName = chr.boxname;
	aref ar;
	makearef(ar,Locations[locidx].(atrName));
	if(GetAttributesNum(ar) == 0)
	{
		Locations[locidx].(atrName).Money = 0;
		makearef(ar,Locations[locidx].(atrName));
	}
	int iChance = (85 + GetCharacterSkillToOld(chr, SKILL_FORTUNE));
	if(CheckCharacterPerk(pchar, "BreakAndSilence")) iChance = iChance* 0.5;
	if(CheckCharacterPerk(pchar, "Lockpicking")) iChance = 0;
	// God_hit_us  это такой прикол - задействовать в ловушки для сундуков(boal)
	// токо сундуки и дома
	if (sti(chr.GenQuest.God_hit_us) == 1 && rand(100) >= iChance && !CheckAttribute(pchar, "boxname.closed") && !CheckAttribute(pchar, "boxname.QuestOpened") && !CheckAttribute(pchar, "boxname.QuestClosed"))
	{
		Log_Info("Ловушка!");
		PlaySound("Trapped");
		DoQuestCheckDelay("God_hit_us", 0.2);
	}
	else
	{
		LaunchItemsBox(&ar);
	}
}

void Box_OnLoadLocation(ref _location)
{
	string locatorName;
	aref boxLocator;
	int i;
	bool isAbordageBox = false;
	if (CheckAttribute(_location, "boarding"))
	{
		if (_location.boarding == "true")
		{
            if (isShipInside(_location.id) || _location.id == "Deck_Near_Ship")// boal fix 08.01.04
            {
                isAbordageBox = false;
            }
            else
            {
	            isAbordageBox = true;
            }
		}
	}

       
	// private box
	for (i=1; i < MAX_HANDLED_BOXES; i++)
	{
		locatorName = "private"+i;
		if (!FindLocator(_location.id, locatorName, &boxlocator, true))
		{
			break;
		}

		if (!CheckAttribute(_location, locatorName))
		{
			_location.(locatorName) = "";
			_location.(locatorName).open = true;
		}
	
		// Warship генерим предметы в сундуке
		if(!CheckAttribute(_location, locatorName + ".opened")) 
		{
			makearef(boxLocator, _location.(locatorName));
			FillGenerableItemsForChest(boxLocator);
		}

	}

	// simple box
	for (i=1; i < MAX_HANDLED_BOXES; i++)
	{
		locatorName = "box"+i;
		if (!FindLocator(_location.id, locatorName, &boxlocator, true))
		{
			break;
		}

		// found box
		bool needRespawn;
		// boal -->
		//needRespawn = true;
	    needRespawn = SetLocationQuestBox(_location, locatorName);
		
		if (!needRespawn)
		{
		// boal <--
    		if (!CheckAttribute(_location, (locatorName)))
    		{
    			needRespawn = true;
    			_location.(locatorName) = Items_MakeTime(GetTime(), GetDataDay(), GetDataMonth(), GetDataYear());
    		}
    		else
    		{
    			if ((sti(_location.(locatorName)) + SPAWN_TIME) < Items_MakeTime(GetTime(), GetDataDay(), GetDataMonth(), GetDataYear()))
    			{
    				needRespawn = true;
    				_location.(locatorName) = Items_MakeTime(GetTime(), GetDataDay(), GetDataMonth(), GetDataYear());
    			}
    			else
    			{
    				needRespawn = false;
    			}
    		}
            if (isShipInside(_location.id) || _location.id == "Deck_Near_Ship")// boal fix 08.01.04
            {
                needRespawn = false;
            }
            
    		//respawn items in box
    		if (needRespawn || isAbordageBox)
    		{
    			//clear items in box
    			if (CheckAttribute(_location, locatorName))
    			{
    				DeleteAttribute(_location, locatorName + ".items");
    				DeleteAttribute(_location, locatorName + ".Treasure"); // признак сокровища в сундуке boal
				    _location.(locatorName).items = "";
				    _location.(locatorName).money = 0;
					
					DeleteAttribute(_location, locatorName + ".check");
					
					if(CheckAttribute(_location, locatorName+".GenClosed"))
					{
						DeleteAttribute(_location, locatorName + ".GenClosed");
						DeleteAttribute(_location, locatorName + ".closed");
    			    	DeleteAttribute(_location, locatorName + ".key");
					}
    			}

    			//fill box with new items
    			string goodItemName;
    			int spawnItemsCount = OBJECTS_IN_BOX;
    			ref chr = GetMainCharacter();
				aref rItem;
    			spawnItemsCount = spawnItemsCount + spawnItemsCount * GetCharacterSkillToOld(chr, "Fortune") / 10.0;
    			for (int j=0; j<spawnItemsCount; j++)
    			{
    				if (needRespawn || isAbordageBox)
    				{
    					string itemID;
    					if (SpawnItem(chr, &itemID, isAbordageBox, GetCharacterSkillToOld(chr, "Fortune")))
    					{
							Items_FindItem(itemID, &rItem);
    						if (CheckAttribute(_location, locatorName+".items."+itemID))
    						{
    							_location.(locatorName).items.(itemID) = sti(_location.(locatorName).items.(itemID)) + 1;
    							if(rand(1) == 1) _location.(locatorName).items.(itemID) = sti(_location.(locatorName).items.(itemID)) +  (1 + rand(sti(rItem.maxChestQuantity)) - 1);
    						}
    						else
    						{
    							_location.(locatorName).items.(itemID) = 1;
								if(rand(1) == 1) _location.(locatorName).items.(itemID) = sti(_location.(locatorName).items.(itemID)) +  (1 + rand(sti(rItem.maxChestQuantity)) - 1);
    						}
							
							if(sti(_location.(locatorName).items.(itemID)) > sti(rItem.maxChestQuantity)) _location.(locatorName).items.(itemID) = rItem.maxChestQuantity;
    					}
    				}
    			}
				
				if(CheckAttribute(_location, (locatorName) + ".GenQuestBoxOpened"))
				{
					DeleteAttribute(_location, locatorName + ".GenQuestBoxOpened");
					_location.(locatorName).QuestOpened = true;
				}
    		}//respawn
		}
		
		if (needRespawn || isAbordageBox) // это должно быть здесь когда уже известно значение переменной needRespawn
		{
			// Warship генерим предметы в сундуке
			makearef(boxLocator, _location.(locatorName));
			FillGenerableItemsForChest(boxLocator);
		}

	}
}

// Warship Для новой системы - перебор итемов для генерации, т.к. везде ложится по-старинке - items.(itemID) = n, а генерить нужно
void FillGenerableItemsForChest(aref _chest)
{
	int i, count;
	String itemID;
	
	aref chestItems;
	aref item;
	
	makearef(chestItems, _chest.items);
	
	for(i = 0; i < GetAttributesNum(chestItems); i++)
	{
		item = GetAttributeN(chestItems, i);
		itemID = GetAttributeName(item);
		
		if(IsGenerableItem(itemID))
		{
			count = sti(chestItems.(itemID));
			DeleteAttribute(chestItems, itemID);
			
			itemID = GetGeneratedItem(itemID);
			chestItems.(itemID) = count;
		}
	}
}

void FillClosedChestForItems(aref loc, aref _chest)
{
	DeleteAttribute(_chest, "items");
	DeleteAttribute(_chest, "money");
	
	int irand;
	
	if(CheckAttribute(loc, "id.label"))
	{
		switch (loc.id.label)
	    {
	    	case "House":
			    irand = rand(3000);
			    _chest.money = irand;
				irand = rand(1);
				if(irand == 0) GenerateAndAddItems(_chest, "mineral1", 1);
				irand = rand(2);
				if(irand == 1) GenerateAndAddItems(_chest, "mineral2", rand(3)+1);
				irand = rand(3);
				if(irand == 0) GenerateAndAddItems(_chest, "mineral3", 1);
				irand = rand(4);
				if(irand == 1) GenerateAndAddItems(_chest, "mineral8", 1);
				irand = rand(5);
				if(irand == 0) GenerateAndAddItems(_chest, "mineral10", 1);
				irand = rand(6);
				if(irand == 1) GenerateAndAddItems(_chest, "mineral11", 1);
				irand = rand(7);
				if(irand == 0) GenerateAndAddItems(_chest, "jewelry1", 1);
				irand = rand(8);
				if(irand == 1) GenerateAndAddItems(_chest, "jewelry6", rand(2)+1);
	    	break;
			
			case "Room":
			    irand = rand(500);
			    _chest.money = irand;
				irand = rand(1);
				if(irand == 1) GenerateAndAddItems(_chest, "mineral1", 1);
				irand = rand(2);
				if(irand == 0) GenerateAndAddItems(_chest, "mineral5", 1);
				irand = rand(3);
				if(irand == 1) GenerateAndAddItems(_chest, "mineral7", 1);
				irand = rand(4);
				if(irand == 0) GenerateAndAddItems(_chest, "mineral8", 1);
				irand = rand(5);
				if(irand == 1) GenerateAndAddItems(_chest, "mineral15", 1);
				irand = rand(6);
				if(irand == 0) GenerateAndAddItems(_chest, "jewelry5", rand(2)+1);
	    	break;
			
			case "packhouse":
			    irand = rand(1);
			    if(irand == 0) _chest.money = rand(250);
				else _chest.money = 0;
				irand = rand(2);
				if(irand == 1) GenerateAndAddItems(_chest, "blade_19", 1);
				irand = rand(3);
				if(irand == 0) GenerateAndAddItems(_chest, "topor_04", 1);
				irand = rand(4);
				if(irand == 1) GenerateAndAddItems(_chest, "spyglass1", 1);
				irand = rand(5);
				if(irand == 0) GenerateAndAddItems(_chest, "jewelry5", rand(2)+1);
	    	break;
			
			case "Bedroom":
			    _chest.money = rand(1000);
				irand = rand(1);
				if(irand == 0) GenerateAndAddItems(_chest, "gold_dublon", rand(5));
				irand = rand(2);
				if(irand == 1) GenerateAndAddItems(_chest, "mineral1", 1);
				irand = rand(3);
				if(irand == 0) GenerateAndAddItems(_chest, "mineral5", 1);
				irand = rand(4);
				if(irand == 1) GenerateAndAddItems(_chest, "mineral7", 1);
				irand = rand(5);
				if(irand == 0) GenerateAndAddItems(_chest, "mineral8", 1);
				irand = rand(6);
				if(irand == 1) GenerateAndAddItems(_chest, "mineral15", 1);
				irand = rand(7);
				if(irand == 0) GenerateAndAddItems(_chest, "jewelry2", rand(1)+1);
				irand = rand(8);
				if(irand == 1) GenerateAndAddItems(_chest, "jewelry3", rand(1)+1);
				irand = rand(9);
				if(irand == 0) GenerateAndAddItems(_chest, "jewelry4", rand(1)+1);
	    	break;
			
			case "Crypt":
			    irand = rand(1);
			    if(irand == 0) _chest.money = rand(100);
				else _chest.money = 0;
				irand = rand(2);
				if(irand == 0) GenerateAndAddItems(_chest, "gold_dublon", rand(5));
				irand = rand(3);
				if(irand == 1) GenerateAndAddItems(_chest, "jewelry7", 1);
				irand = rand(4);
				if(irand == 0) GenerateAndAddItems(_chest, "jewelry8", 1);
	    	break;
			
			case "shack":
			    _chest.money = 0;
				Log_Testinfo("Придумать, что пихать в закрытые сундуки в лачугах!")
	    	break;
	    }
	}
	else
	{
		_chest.money = rand(250);
	}
	
	//_chest.money = rand(2500);
}

bool SpawnItem(ref _chr, ref _id, bool isAbordageBox, float luck)
{
	int chrRank = sti(_chr.rank);
	int itemRank = 0;
	int perkMltPlr = 1;
	aref randItem;
	string sItem = SelectRandItem("Chest", "");
	
	Items_FindItem(sItem, &randItem);
	
	if (randItem.index == -1)
	{
		return false;
	}
	
	if (CheckAttribute(randItem, "minLevel"))
	{
		itemRank = sti(randItem.minLevel);
	}
	
	if (itemRank > chrRank)
	{
		return false;
	}

	float itemProb = 0.05;
	if (CheckAttribute(randItem, "rare"))
	{
		switch(randItem.rare)
	    {
	        case "common":    itemProb = 0.1 + fRandSmall(0.2);     break;
        	case "rare":      itemProb = 0.01 + fRandSmall(0.02);   break;
    		case "epic":      itemProb = 0.001 + fRandSmall(0.002); break;
    		case "Legendary": itemProb = 0.0001;                    break;
    	}
	}
	
	if(CheckCharacterPerk(pchar, "TreasureHunter")) perkMltPlr = 2;
	
	if(CheckCharacterPerk(pchar, "Lockpicking")) perkMltPlr = 4;
	
	if(HasSubStr(randItem.id, "jewelry") || randItem.id == "gold_dublon" || randItem.id == "gold" || randItem.rare == "Epic") itemProb = itemProb * perkMltPlr;
	
	if(CheckCharacterPerk(pchar, "ThriftyJaeger"))
	{
		if(randItem.groupID == AMMO_ITEM_TYPE) itemProb = itemProb * 1.7;
	}
	
	if (isAbordageBox)
	{
		itemProb = itemProb * (1.5 + luck/20.0);
	}
	
	if (itemProb > 1.0)
	{
		itemProb = 1.0;
	}
	int rndNumber = rand(1000);
	if (rndNumber > (itemProb*1000))
	{
		return false;
	}
	
	if(CheckAttribute(randItem, "rare") && randItem.rare == "Legendary")
	{
		return false;
	}
	
	// отличные клинки в сундуках больше не генерятся <-- ugeen
	if(CheckAttribute(randItem,"quality") && randItem.quality == "excellent")
	{
		return false;
	}
	
	_id = GetGeneratedItem(randItem.id);
	return true;
}

object g_TmpModelVariable; // код от к3, в скриптах нет вообще, есть проверка в ядре