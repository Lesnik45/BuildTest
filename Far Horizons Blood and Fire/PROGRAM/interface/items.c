// BOAL меню предметов
#include "interface\character_all.h"

void InitInterface(string iniName)
{
    InterfaceStack.SelectMenu_node = "LaunchItems"; // запоминаем, что звать по Ф2
	GameInterface.title = "titleItems";
	
	xi_refCharacter = pchar;
	
	FillCharactersScroll();
	
	SendMessage(&GameInterface,"ls",MSG_INTERFACE_INIT,iniName);

	SetEventHandler("InterfaceBreak","ProcessExitCancel",0);
	SetEventHandler("exitCancel","ProcessExitCancel",0);
    SetEventHandler("ievnt_command","ProcessCommandExecute",0);
    SetEventHandler("frame","ProcessFrame",1);
    SetEventHandler("ShowInfoWindow","ShowInfoWindow",0);
	SetEventHandler("MouseRClickUp","HideInfoWindow",0);
	SetEventHandler("TableSelectChange", "TableSelectChange", 0);
	SetEventHandler("eTabControlPress","procTabChange",0);
	SetEventHandler("ExitMapWindow","ExitMapWindow",0);
    
    XI_RegistryExitKey("IExit_F2");
    SetVariable();
    SetNewGroupPicture("Weight_PIC", "ICONS_CHAR", "weight");
    SetNewGroupPicture("Money_PIC", "ICONS_CHAR", "Money");
}

void ProcessExitCancel()
{
	IDoExit(RC_INTERFACE_ANY_EXIT);
}

void IDoExit(int exitCode)
{
	DelEventHandler("InterfaceBreak","ProcessExitCancel");
	DelEventHandler("exitCancel","ProcessExitCancel");
    DelEventHandler("ievnt_command","ProcessCommandExecute");
    DelEventHandler("frame","ProcessFrame");
    DelEventHandler("ShowInfoWindow","ShowInfoWindow");
	DelEventHandler("MouseRClickUp","HideInfoWindow");
	DelEventHandler("TableSelectChange", "TableSelectChange");
	DelEventHandler("eTabControlPress","procTabChange");
	DelEventHandler("ExitMapWindow","ExitMapWindow");

	interfaceResultCommand = exitCode;
	if( CheckAttribute(&InterfaceStates,"ReloadMenuExit"))
	{
        DeleteAttribute(&InterfaceStates,"ReloadMenuExit");
		EndCancelInterface(false);
	}
	else
	{
		EndCancelInterface(true);
	}
}

void ProcessCommandExecute()
{
	string comName = GetEventData();
	string nodName = GetEventData();
	
    switch(nodName)
	{
		case "EQUIP_BUTTON":
			if (comName=="activate" || comName=="click")
			{
				EquipPress();
			}
		break;
		
		case "I_CHARACTER_2":
			if(comName=="click")
			{
			    nodName = "I_CHARACTER";
			}
		break;
		case "I_SHIP_2":
			if(comName=="click")
			{
			    nodName = "I_SHIP";
			}
		break;
		case "I_QUESTBOOK_2":
			if(comName=="click")
			{
			    nodName = "I_QUESTBOOK";
			}
		break;
		case "I_TRADEBOOK_2":
			if(comName=="click")
			{
			    nodName = "I_TRADEBOOK";
			}
		break;
		case "I_NATIONS_2":
			if(comName=="click")
			{
			    nodName = "I_NATIONS";
			}
		break;
		case "I_ITEMS_2":
			if(comName=="click")
			{
			    nodName = "I_ITEMS";
			}
		break;
	}
	// boal new menu 31.12.04 -->
	if (nodName == "I_CHARACTER" || nodName == "I_SHIP" ||
	    nodName == "I_QUESTBOOK" || nodName == "I_TRADEBOOK" ||
		nodName == "I_NATIONS" || nodName == "I_ITEMS")
	{
		if(comName=="click")
		{
            InterfaceStates.ReloadMenuExit = true;
			IDoExit(RC_INTERFACE_ANY_EXIT);
			PostEvent("LaunchIAfterFrame",1,"sl", nodName, 2);
			return;
		}
	}
	// boal new menu 31.12.04 -->
}

void ProcessFrame()
{
	if (sti(GameInterface.CHARACTERS_SCROLL.current)!=nCurScrollNum && GetCurrentNode() == "CHARACTERS_SCROLL")
	{
		nCurScrollNum = sti(GameInterface.CHARACTERS_SCROLL.current);
		SetButtonsState();
		return;
	}
}

void SetButtonsState()
{
	string attributeName = "pic" + (nCurScrollNum+1);
	
	if(GameInterface.CHARACTERS_SCROLL.(attributeName).character != "0")
	{
		int iCharacter = sti(GameInterface.CHARACTERS_SCROLL.(attributeName).character);
		xi_refCharacter = &characters[iCharacter];
		SetVariable();
	}
	else
	{
        xi_refCharacter = pchar;
        SetVariable();
	}
}

void SetVariable()
{
	SetFormatedText("SETUP_FRAME_CAPTION", XI_ConvertString("Equipment") + ": " + GetFullName(xi_refCharacter));
	// сортировка -->
	SortItems(xi_refCharacter);
	// сортировка -->
	SetControlsTabMode(1);
	HideItemInfo();
	SetFormatedText("Weight_TEXT", XI_ConvertString("weight") + ": " + FloatToString(GetItemsWeight(xi_refCharacter), 1) + " / "+GetMaxItemsWeight(xi_refCharacter));
	SetFormatedText("Money_TEXT", MakeMoneyShow(sti(xi_refCharacter.Money), MONEY_SIGN,MONEY_DELIVER));
	
	SetNodeUsing("EQUIP_BUTTON" , true);
	SendMessage(&GameInterface,"lsls",MSG_INTERFACE_MSG_TO_NODE,"EQUIP_BUTTON",0, "#"+XI_ConvertString("Equip that"));
}

void FillItemsTable(int _mode)
{
	int n, i;
	string row;
	string sGood;
	string groupID;
	string itemType;
	int  idLngFile;
	bool ok, ok1, ok2, ok3;
	aref rootItems, arItem;
	aref  curItem;
	
	GameInterface.TABLE_ITEMS.hr.td1.str = "Наименование предметов";
	GameInterface.TABLE_ITEMS.hr.td1.scale = 0.8;
	GameInterface.TABLE_ITEMS.hr.td2.str = "Вес шт.";
	GameInterface.TABLE_ITEMS.hr.td2.scale = 0.8;
	GameInterface.TABLE_ITEMS.hr.td3.str = "Кол-во";
	GameInterface.TABLE_ITEMS.hr.td3.scale = 0.8;
	GameInterface.TABLE_ITEMS.hr.td4.str = "Вес";
	GameInterface.TABLE_ITEMS.hr.td4.scale = 0.8;
	GameInterface.TABLE_ITEMS.select = 0;
	GameInterface.TABLE_ITEMS.top = 0;
	n = 1;
	idLngFile = LanguageOpenFile("ItemsDescribe.txt");
	Table_Clear("TABLE_ITEMS", false, true, false);
	
	// Заполним вещами от нас
	makearef(rootItems, xi_refCharacter.Items);
    for (i=0; i<GetAttributesNum(rootItems); i++)
    {
		curItem = GetAttributeN(rootItems, i);
		groupID = "";
		itemType = "";

		if (Items_FindItem(GetAttributeName(curItem), &arItem)>=0 )
		{
			row = "tr" + n;
			sGood = arItem.id;
			
			// Hokkins: новая сортировка предметов -->
			if(CheckAttribute(arItem,"groupID")) groupID = arItem.groupID;
			if(CheckAttribute(arItem,"itemType")) itemType = arItem.itemType;
			
			// Снаряжение -->
			ok = (groupID == BLADE_ITEM_TYPE) || 	// холодное оружие
				 (groupID == GUN_ITEM_TYPE)	||		// огнестрельное оружие
                 (groupID == SPYGLASS_ITEM_TYPE) || // подзорные трубы
				 (groupID == CIRASS_ITEM_TYPE) ||   // костюмы и доспехи
				 (groupID == AMMO_ITEM_TYPE) ||        // боеприпасы
				 (groupID == TALISMAN_ITEM_TYPE);        // талисманы
				 
			// Зелья -->
			ok1 = HasSubStr(arItem.id, "potion");
			
			// Важное -->
			ok2 = (itemType == "QUESTITEMS") || 	// квестовые предметы
				  (arItem.id == "Map_Best");        // отличная карта
			
			// Разное -->
			ok3 = (HasSubStr(arItem.id, "mineral"));            // хламовые предметы
			
			//if(_mode == 1 && groupID == MAPS_ITEM_TYPE && arItem.id != "MapsAtlas") continue;
			if(_mode == 2 && !ok) continue;
			if(_mode == 3 && !ok1) continue;
			if(_mode == 4 && !ok2) continue;
			if(_mode == 5 && !ok3) continue;
			
			if (GetCharacterItem(xi_refCharacter, sGood) > 0)
			{		
				GameInterface.TABLE_ITEMS.(row).index = GetItemIndex(arItem.id);
				
				GameInterface.TABLE_ITEMS.(row).td1.icon.group = arItem.picTexture;
				GameInterface.TABLE_ITEMS.(row).td1.icon.image = "itm" + arItem.picIndex;
				GameInterface.TABLE_ITEMS.(row).td1.icon.offset = "2, 0";
				GameInterface.TABLE_ITEMS.(row).td1.icon.width = 32;
				GameInterface.TABLE_ITEMS.(row).td1.icon.height = 32;
				GameInterface.TABLE_ITEMS.(row).td1.textoffset = "31,0";
				if (findsubstr(arItem.id, "recipe_" , 0) != -1) GameInterface.TABLE_ITEMS.(row).td1.str = LanguageConvertString(idLngFile, arItem.name) + " (" + LanguageConvertString(idLngFile, "Itmname_"+arItem.result) + ")";
				else GameInterface.TABLE_ITEMS.(row).td1.str = LanguageConvertString(idLngFile, arItem.name);
				GameInterface.TABLE_ITEMS.(row).td1.scale = 0.85;
				
				GameInterface.TABLE_ITEMS.(row).td2.str   = FloatToString(stf(arItem.Weight), 1);
				GameInterface.TABLE_ITEMS.(row).td2.scale = 0.9;
				GameInterface.TABLE_ITEMS.(row).td3.str   = GetCharacterItem(xi_refCharacter, sGood);
				GameInterface.TABLE_ITEMS.(row).td3.scale = 0.9;
				GameInterface.TABLE_ITEMS.(row).td4.str   = FloatToString(stf(arItem.Weight) * sti(GameInterface.TABLE_ITEMS.(row).td3.str), 1);
				GameInterface.TABLE_ITEMS.(row).td4.scale = 0.9;
				n++;
			}
		}
    }
    
	Table_UpdateWindow("TABLE_ITEMS");
	LanguageCloseFile(idLngFile);
	if (_mode == 1)
	{
		FillItemsSelected();
	}
}

void FillItemsSelected() 
{
	int    i;
	string sGood;
	int iLastGunItem;
	ref rLastGunItem;
	
	// Скроем по умолчанию
	SetNodeUsing("ITEM_1", false);
	SetNodeUsing("ITEM_2", false);
	SetNodeUsing("ITEM_3", false);
	SetNodeUsing("ITEM_4", false);
	SetNodeUsing("ITEM_5", false);
	
    for (i = 0; i< TOTAL_ITEMS; i++)
	{
		if(!CheckAttribute(&Items[i], "ID"))
		{
			continue;
		}
		
		sGood = Items[i].id;
		
		if (GetCharacterItem(xi_refCharacter, sGood) > 0)
		{		
			/// экипировка
			if (IsEquipCharacterByItem(xi_refCharacter, sGood))
			{
				switch (Items[i].groupID) 
				{
					case BLADE_ITEM_TYPE:
						SetNewGroupPicture("ITEM_1", Items[i].picTexture, "itm" + Items[i].picIndex);
						SetNodeUsing("ITEM_1" , true);
					break;
					case GUN_ITEM_TYPE:
						if(CheckAttribute(xi_refCharacter, "IsMushketer"))
						{
							iLastGunItem = GetItemIndex(xi_refCharacter.IsMushketer.LastGunID);
							// Покажем картинку старого пистоля, если он еще есть
							if(iLastGunItem != -1 && GetCharacterItem(xi_refCharacter, xi_refCharacter.IsMushketer.LastGunID) > 0)
							{
								rLastGunItem = &Items[iLastGunItem];
								SetNewGroupPicture("ITEM_2", rLastGunItem.picTexture, "itm" + rLastGunItem.picIndex);
								SetNodeUsing("ITEM_2" , true);
							}
							
							rLastGunItem = &Items[GetItemIndex(xi_refCharacter.IsMushketer.MushketID)];
							SetNodeUsing("ITEM_5", true);
							SetNewGroupPicture("ITEM_5", rLastGunItem.picTexture, "itm" + rLastGunItem.picIndex);
						}
						else
						{
							SetNewGroupPicture("ITEM_2", Items[i].picTexture, "itm" + Items[i].picIndex);
							SetNodeUsing("ITEM_2" , true);
							SetNodeUsing("ITEM_5" , false); // Мушкет не юзается - уберем картинку
						}
					break;
					case SPYGLASS_ITEM_TYPE:
						SetNewGroupPicture("ITEM_3", Items[i].picTexture, "itm" + Items[i].picIndex);
						SetNodeUsing("ITEM_3" , true);
					break;
					case CIRASS_ITEM_TYPE:
						SetNewGroupPicture("ITEM_4", Items[i].picTexture, "itm" + Items[i].picIndex);
						SetNodeUsing("ITEM_4" , true);
					break;
				}
			}
		}			
	}
}

void ShowInfoWindow()
{
	string sCurrentNode = GetCurrentNode();
	string sHeader, sText1, sText2, sText3, sPicture;
	string sGroup, sGroupPicture;
	int iItem;

	sPicture = "-1";
	string sAttributeName;
	int nChooseNum = -1;
	switch (sCurrentNode)
	{
        
	}
	sHeader = "Предметы";
	sText1  = "В этом интерфейсе Вы можете просмотреть и назначить герою саблю, пистолет, подзорную трубу и кирасу. "+
	          "Так же можно посмотреть состояние инвентаря офицеров, компаньонов и пассажиров. Выбор оружия они делают сами при экипировке их новым снаряжением. "+
			  "Обмен предметами со своими офицерами осуществляется на суше через команду обмен.";
	CreateTooltip("#" + sHeader, sText1, argb(255,255,255,255), sText2, argb(255,255,192,192), sText3, argb(255,192,255,192), "", argb(255,255,255,255), sPicture, sGroup, sGroupPicture, 64, 64);

}

void HideInfoWindow()
{
	CloseTooltip();
}

void TableSelectChange()
{
	string sControl = GetEventData();
	iSelected = GetEventData();
    CurTable = sControl;
    CurRow   =  "tr" + (iSelected);
    
    // отрисовка инфы
    SetItemInfo();
}

void SetItemInfo()
{
	int iGoodIndex = sti(GameInterface.(CurTable).(CurRow).index);
	
	SetFormatedText("INFO_TEXT", GetItemDescribe(iGoodIndex));
	SetNewGroupPicture("INFO_PIC", Items[iGoodIndex].picTexture, "itm" + Items[iGoodIndex].picIndex);
	SetNodeUsing("INFO_TEXT", true);
	SetNodeUsing("INFO_PIC", true);
	SetVAligmentFormatedText("INFO_TEXT");
	
	SendMessage(&GameInterface,"lsls",MSG_INTERFACE_MSG_TO_NODE,"EQUIP_BUTTON",0, "#"+XI_ConvertString("Equip that"));
	SetSelectable("EQUIP_BUTTON",ThisItemCanBeEquip(&Items[iGoodIndex]));
}

void HideItemInfo()
{
	SetNodeUsing("INFO_TEXT", false);
	SetNodeUsing("INFO_PIC", false);
	SendMessage(&GameInterface,"lsls",MSG_INTERFACE_MSG_TO_NODE,"EQUIP_BUTTON",0, "#"+XI_ConvertString("Equip that"));
	SetSelectable("EQUIP_BUTTON",false);
}

void procTabChange()
{
	int iComIndex = GetEventData();
	string sNodName = GetEventData();
	SetCurrentNode("TABLE_ITEMS");
	HideItemInfo();
	if( sNodName == "TABBTN_1" )
	{
		SetControlsTabMode( 1 );
		return;
	}
	if( sNodName == "TABBTN_2" )
	{
		SetControlsTabMode( 2 );
		return;
	}
	if( sNodName == "TABBTN_3" )
	{
		SetControlsTabMode( 3 );
		return;
	}
	if( sNodName == "TABBTN_4" )
	{
		SetControlsTabMode( 4 );
		return;
	}
	if( sNodName == "TABBTN_5" )
	{
		SetControlsTabMode( 5 );
		return;
	}
}

void SetControlsTabMode(int nMode)
{
	int nColor1 = argb(255,196,196,196);
	int nColor2 = nColor1;
	int nColor3 = nColor1;
	int nColor4 = nColor1;
	int nColor5 = nColor1;

	string sPic1 = "TabSelected";
	string sPic2 = sPic1;
	string sPic3 = sPic1;
	string sPic4 = sPic1;
	string sPic5 = sPic1;

	switch (nMode)
	{
		case 1: //
			sPic1 = "TabDeSelected";
			nColor1 = argb(255,255,255,255);
		break;
		case 2:
			sPic2 = "TabDeSelected";
			nColor2 = argb(255,255,255,255);
		break;
		case 3:
			sPic3 = "TabDeSelected";
			nColor3 = argb(255,255,255,255);
		break;
		case 4:
			sPic4 = "TabDeSelected";
			nColor4 = argb(255,255,255,255);
		break;
		case 5:
			sPic5 = "TabDeSelected";
			nColor5 = argb(255,255,255,255);
		break;
	}
    
	SetNewGroupPicture("TABBTN_1", "TABS", sPic1);
	SetNewGroupPicture("TABBTN_2", "TABS", sPic2);
	SetNewGroupPicture("TABBTN_3", "TABS", sPic3);
	SetNewGroupPicture("TABBTN_4", "TABS", sPic4);
	SetNewGroupPicture("TABBTN_5", "TABS", sPic5);
	SendMessage(&GameInterface,"lslll",MSG_INTERFACE_MSG_TO_NODE,"TABSTR_1", 8,0,nColor1);
	SendMessage(&GameInterface,"lslll",MSG_INTERFACE_MSG_TO_NODE,"TABSTR_2", 8,0,nColor2);
    SendMessage(&GameInterface,"lslll",MSG_INTERFACE_MSG_TO_NODE,"TABSTR_3", 8,0,nColor3);
    SendMessage(&GameInterface,"lslll",MSG_INTERFACE_MSG_TO_NODE,"TABSTR_4", 8,0,nColor4);
	SendMessage(&GameInterface,"lslll",MSG_INTERFACE_MSG_TO_NODE,"TABSTR_5", 8,0,nColor5);
	FillControlsList(nMode);
}

void FillControlsList(int nMode)
{
	switch (nMode)
	{
	    case 1: FillItemsTable(1); break;  // все
	    case 2: FillItemsTable(2); break;  // снаряжение
	    case 3: FillItemsTable(3); break;  // зелья
	    case 4: FillItemsTable(4); break;  // важное
		case 5: FillItemsTable(5); break;  // разное
	}
}

bool ThisItemCanBeEquip( aref arItem )
{
	if( !CheckAttribute(arItem,"groupID") )
	{
		return false;
	}
	if( !IsCanEquiping(xi_refCharacter, arItem.groupID) )
	{
		return false;
	}
	if(arItem.id == "MapsAtlas") 
	{
		return true;
	}
	
	if (arItem.groupID == AMMO_ITEM_TYPE) 
	{
		if (arItem.ID == "GunPowder") return false;
	}
	
	if (arItem.groupID == GUN_ITEM_TYPE) 
	{
		if (!CheckAttribute(arItem,"chargeQ") )
		{
			return false;
		}
		int chrgQ = sti(arItem.chargeQ);
	
		if (chrgQ >= 2 && !IsCharacterPerkOn(xi_refCharacter,"Musketeer") )
		{
			return false;
		}
		
		if(arItem.id == "mushket2x2") return false; // Мушкет квестового офа
		
		// Для мушкетов нужен соответствующий перк
		if(HasSubStr(arItem.id, "mushket") && !IsCharacterPerkOn(xi_refCharacter,"Breter"))
		{
			return false;
		}
		
		// Нельзя экипировать мушкет в непредназначенных для этого локациях (Таверна)
		if(HasSubStr(arItem.id, "mushket") && !CanEquipMushketOnLocation(PChar.Location))
		{
			return false;
		}
		
		// Hokkins: если это не наш главный герой, то мушкет экипировать нельзя
		if(HasSubStr(arItem.id, "mushket") && !IsMainCharacter(xi_refCharacter))
		{
			return false;
		}
	}
    
    if (IsEquipCharacterByItem(xi_refCharacter, arItem.id))
	{
		SendMessage(&GameInterface,"lsls",MSG_INTERFACE_MSG_TO_NODE,"EQUIP_BUTTON",0, "#"+XI_ConvertString("Remove that"));
	}
	else
	{
		if (arItem.groupID == CIRASS_ITEM_TYPE && arItem.Clothes == false)
	    {
			if(arItem.id == "cirass4" || arItem.id == "cirass5")
			{
				if(!IsCharacterPerkOn(xi_refCharacter, "Knight")) return false;
			}
			
			if(arItem.id == "cirass2" || arItem.id == "cirass3")
			{
				if(!IsCharacterPerkOn(xi_refCharacter, "BasicDefense")) return false;
			}
		    //return false;
	    }
		SendMessage(&GameInterface,"lsls",MSG_INTERFACE_MSG_TO_NODE,"EQUIP_BUTTON",0, "#"+XI_ConvertString("Equip that"));
	}
	
	if(CheckAttribute(PChar, "IsMushketer"))
	{
		if(arItem.ID == PChar.IsMushketer.MushketID)
		{
			SendMessage(&GameInterface,"lsls",MSG_INTERFACE_MSG_TO_NODE,"EQUIP_BUTTON",0, "#"+XI_ConvertString("Remove that"));
			return true;
		}
		
		if(arItem.groupID == BLADE_ITEM_TYPE || arItem.groupID == SPYGLASS_ITEM_TYPE || arItem.groupID == GUN_ITEM_TYPE || arItem.groupID == CIRASS_ITEM_TYPE)
		{
			return false;
		}
	}
	
	return true;
}

void EquipPress()
{
	int  iGoodIndex = sti(GameInterface.(CurTable).(CurRow).index);
	ref itmRef = &Items[iGoodIndex];
	string totalInfo;
	int  i;
	
	if (CheckAttribute(itmRef, "groupID"))
	{
		string itmGroup = itmRef.groupID;
		switch (itmGroup)
		{
	    	case SPECIAL_ITEM_TYPE:
			    if (findsubstr(itmRef.id, "recipe_" , 0) != -1) // рецепты
				{
					if (isMultiObjectKnown(itmRef.result))
					{
						PlaySound("interface\knock.wav");
						log_info("Вам уже известен этот рецепт!");
					}
					else
					{
						RemoveItems(pchar, itmRef.id, 1);
						AddQuestRecordInfo("Recipe", itmRef.result);
						SetAlchemyRecipeKnown(itmRef.result);
					}
				}
				
				if (itmRef.id == "purse1") // кошель
				{
					int irand = (rand(91)+12)
					RemoveItems(pchar, itmRef.id, 1);
					TakeNItems(pchar, "gold_dublon", irand);
					PlaySound("Ambient\Tavern\monetki_taverna_001.wav");
					log_info("Получено "+irand+" дублонов");
				}
				
				if(CheckAttribute(itmRef, "potion.special") && itmRef.potion.special.effect != "")
				{
					if(CheckAttribute(xi_refCharacter, "SpeciaPotionEffect."+itmRef.potion.special.effect))
					{
						PlaySound("interface\knock.wav");
					}
					else
					{
				        DoCharacterUsedItem(xi_refCharacter, itmRef.id);
					}
				}
				
				if (itmRef.id == "Chest_closed" && xi_refCharacter.id == pchar.id)
                {
                    pchar.questTemp.Lockpicking.ID = itmRef.id;
                    IDoExit(RC_INTERFACE_LOCKPICKING);
                    return;
                }
				SetVariable();
				return;
	    	break;
			case POISON_ITEM_TYPE:
			    DoCharacterUsedPoison(xi_refCharacter, itmRef.id);
				
				SetVariable();
				return;
	    	break;
	    	case RATPOISON_ITEM_TYPE:
			    if(!CheckAttribute(xi_refCharacter, "questTemp.arsenic"))
				{
			        xi_refCharacter.questTemp.arsenic = true;
		    		RemoveItems(pchar, itmRef.id, 1);
	    			Log_TestInfo("Мышьяк у персонажа "+xi_refCharacter.id+" активирован!");
    				SetRatPoisonTimerFunctionParam(xi_refCharacter.id + "_RemoveRatPoisonEffectFromCharacter", "RemoveRatPoisonEffectFromCharacter", 0, 0, 30, xi_refCharacter);
				}
				else
				{
					Log_TestInfo("Куда второй мышьяк этому персу-то? ("+xi_refCharacter.id+")");
					PlaySound("interface\knock.wav");
				}
				SetVariable();
				return;
	    	break;
			
			case AMMO_ITEM_TYPE:
				string sGun = GetCharacterEquipByGroup(xi_refCharacter, GUN_ITEM_TYPE);
				if (itmRef.ID == "bullet" || itmRef.ID == "cartridge" || itmRef.ID == "silver_bullet" || itmRef.ID == "Silver_cartridge")
				{
					if (sGun == "pistol2" || sGun == "pistol4" || sGun == "pistol8" || sGun == "pistol9" || sGun == "mushket_Arquebus" || sGun == "mushket_Portuguese")
					{
						PlaySound("interface\knock.wav");
						return;
					}
				}
				if (itmRef.ID == "grapeshot"|| itmRef.ID == "gunechin")
				{	
					if (sGun != "pistol2" && sGun != "pistol4" && sGun != "pistol9" && sGun != "mushket_Arquebus" && sGun != "mushket_Portuguese")
					{
						PlaySound("interface\knock.wav");
						return;
					}
				}
				if (itmRef.ID == "petard" || itmRef.ID == "grenade" || itmRef.ID == "Poison_bomb")
				{	
					if (sGun != "mushket_sklopetta" && sGun != "mushket_Bombard")
					{
						PlaySound("interface\knock.wav");
						return;
					}
				}
				LAi_SetCharacterUseBullet(xi_refCharacter, itmRef.ID);
				LAi_GunSetUnload(xi_refCharacter);
				log_info("Выбран боеприпас - "+GetConvertStr(itmRef.name, "ItemsDescribe.txt")+"");
				PlaySound("People Fight\reload1.wav");
				RemoveCharacterEquip(xi_refCharacter, itmGroup);
				SetVariable();
				return;
	    	break;
		}
		
		if(HasSubStr(itmRef.id, "Mushket")) // Пытаемся одеть мушкет... на тетку не оденеться, нет анимации
		{
		    if(!CheckAttribute(PChar, "IsMushketer")) // Не мушкетер. Делаем мушкетером
		    {
		        SetMainCharacterToMushketer(itmRef.id, true);
		    }
		    else // Мушкетер. Делаем обычным фехтовальщиком
		    {
		        SetMainCharacterToMushketer("", false);
		    }
		}
		else
		{
    	    if(IsEquipCharacterByItem(xi_refCharacter, itmRef.id))
	        {
			    RemoveCharacterEquip(xi_refCharacter, itmGroup);
		    }
    	    else
	        {
			    EquipCharacterByItem(xi_refCharacter, itmRef.id);
		    }
		}
		FillItemsSelected();
		SendMessage(&GameInterface,"lsls",MSG_INTERFACE_MSG_TO_NODE,"EQUIP_BUTTON",0, "#"+XI_ConvertString("Equip that"));
		SetSelectable("EQUIP_BUTTON",ThisItemCanBeEquip(&Items[iGoodIndex]));
	}
} 

void ExitMapWindow()
{
	XI_WindowShow("MAP_WINDOW", false);
	XI_WindowDisable("MAP_WINDOW", true);
	XI_WindowDisable("MAIN_WINDOW", false);

	SetCurrentNode("TABLE_ITEMS");
}

void ShowMapWindow()
{
	XI_WindowShow("MAP_WINDOW", true);
	XI_WindowDisable("MAP_WINDOW", false);
	XI_WindowDisable("MAIN_WINDOW", true);

	SetCurrentNode("MAP_TEXT");
}