void ProcessDialogEvent()
{
	ref NPChar, her;
	aref Link, NextDiag;
	bool bOk = false;
    bool bOk2;
    
	int Sum, nRel, nDay, iChurchQuest2_Summ;
	ref sld;
	
	// Церковный квест № 2 -->
	if(CheckAttribute(PChar, "GenQuest.ChurchQuest_2.AskContra_1") && !CheckAttribute(PChar, "GenQuest.ChurchQuest_2.Summ_To_Contra"))
	{
		iChurchQuest2_Summ = sti(PChar.rank)*100 + (rand(3)+1)*100;
		PChar.GenQuest.ChurchQuest_2.Summ_To_Contra = sti(iChurchQuest2_Summ);
	}
	// <-- Церковный квест № 2
	
	string sColony;
	
	DeleteAttribute(&Dialog,"Links");

	makeref(NPChar,CharacterRef);
	makearef(Link, Dialog.Links);
	makearef(NextDiag, NPChar.Dialog);
	
    string iDay, iMonth;
	iDay = environment.date.day;
	iMonth = environment.date.month;
	string lastspeak_date = iday + " " + iMonth;

	if (!CheckAttribute(npchar, "quest.trade_date"))
    {
        npchar.quest.trade_date = "";
    }

	//тереть нафиг аттрибут при прошествии дней (navy fix)
	if (CheckAttribute(pchar, "GenQuest.contraTravel.days") && GetQuestPastDayParam("contraTravel") > sti(PChar.GenQuest.contraTravel.days))
	{
		DeleteAttribute(pchar, "GenQuest.contraTravel");
		CloseQuestHeader("Gen_ContrabandTravel");
	}

	switch(Dialog.CurrentNode)
	{
        case "Exit":
			NextDiag.CurrentNode = NextDiag.TempNode;
			DialogExit();
		break;
		
		case "Smuggling_exit":
			NextDiag.CurrentNode = NextDiag.TempNode;
			PlaceSmugglersOnShore(Pchar.quest.contraband.CurrentPlace);
			Pchar.quest.Contraband.active = true;
			pchar.GenQuest.Contraband.GuardNation = npchar.nation;

			ReOpenQuestHeader("Gen_Contraband");
            if (GetIslandByCityName(npchar.city) == "Mein")
            {
            	AddQuestRecord("Gen_Contraband", "t1_1");
            }
            else
            {
	            AddQuestRecord("Gen_Contraband", "t1");
				AddQuestUserData("Gen_Contraband", "sIsland", XI_ConvertString(GetIslandByCityName(npchar.city)));
			}
			AddQuestUserData("Gen_Contraband", "sLoc", GetConvertStr(Pchar.quest.contraband.CurrentPlace, "LocLables.txt"));
				
			DialogExit();
		break;
		
		case "First time":						
			if(NPChar.quest.meeting == "0")
			{
				Dialog.Text = "Что вам надо, капитан? Я не знаю вашего имени, и не могу назвать вам свое.";
				Link.l1 = "Я капитан " + GetFullName(pchar) + ".";
				Link.l1.go = "meeting";
				NPChar.quest.meeting = "1";
			}				
				Dialog.Text = "Что вы привезли на этот раз, капитан?";
				if (LAi_group_GetPlayerAlarm() > 0)
				{
	       			Dialog.Text = RandPhraseSimple("Говори быстрей, "+ GetSexPhrase("приятель","подруга") +", что тебе надо? А то, знаешь ли, погоня за тобой!", "Не медли, быстрее излагай суть дела! За тобой солдатня бегает, не до долгих бесед, знаешь ли...");
				}
				if(FindFirstContrabandGoods(PChar) != -1)
				{
					Link.l1 = "Я привез"+ GetSexPhrase("","ла") +" немного товаров на продажу.";
					Link.l1.go = "Meeting_3";
				}

				if (CheckAttribute(pchar, "GenQuest.contraTravel.active") && sti(pchar.GenQuest.contraTravel.active) == true)
					Link.l2 = "Насчет поездки...";
				else
				{
					Link.l2 = "Мне нужно добраться кое-куда.";
				    Link.l2.go = "Travel";
				}					
				Link.l7 = "Ничего. До встречи.";
				Link.l7.go = "Exit";				
		break;
		
		case "Meeting":		
			Dialog.Text = "Чем могу быть полезен вам, капитан?";
			if(FindFirstContrabandGoods(PChar) != -1)
			{
				Link.l1 = "Я хочу кое-что продать.";
				Link.l1.go = "Meeting_1";
			}
			Link.l2 = "Мне нужно добраться кое-куда.";
			Link.l2.go = "Travel";				
			Link.l5 = "Ничем. Удачи!";
			Link.l5.go = "Exit";				
		break;

		case "Meeting_1":
			Dialog.Text = "Хм... Причем здесь я? Наверное, вы не туда зашли, капитан. Идите в магазин - там с радостью купят то, что вы привезли.";
			Link.l1 = "Боюсь, что мне нужны именно вы.";
			Link.l1.go = "Meeting_2";				
			Link.l2 = "Спасибо за совет. Всего хорошего.";
			Link.l2.go = "exit";				
		break;

		case "Meeting_2":
			Dialog.Text = "Зачем же?";
			Link.l1 = "Я хочу продать то, что не купит ни один торговец на этом острове.";
			Link.l1.go = "Meeting_3";				
		break;

		case "Meeting_3":
			if (GetCompanionQuantity(pchar) > 1 && GetBaseHeroNation() != PIRATE)
			{
				dialog.text = NPCStringReactionRepeat("Сначала избавься от своей эскадры. Она слишком приметная. Мы не можем так рисковать. Приходи на одном корабле, и чтоб он был не больше брига или галеона.", 
					"Я что, непонятно выразился? Повторяю - никакой эскадры!", 
					"Ты что, туп"+ GetSexPhrase("ой","ая") +"? Оди-ин, говорю тебе - оди-ин корабль, не два, не три, а один! Теперь понял"+ GetSexPhrase("","а") +"?",
					"Ох, и как таких дур"+ GetSexPhrase("аков","") +" земля носит...", "block", 1, npchar, Dialog.CurrentNode);
				link.l1 = HeroStringReactionRepeat("Хорошо, я тебя понял"+ GetSexPhrase("","а") +". Один так один.", 
					"Все понятно, просто уточнить хотел"+ GetSexPhrase("","а") +".",
					"Нет, не туп"+ GetSexPhrase("ой","ая") +", просто жадн"+ GetSexPhrase("ый","ая") +" очень. Подумал"+ GetSexPhrase("","а") +", может поменялось что, и можно придти сразу эскадрой...", 
					"Ну ты же видишь - носит как-то...", npchar, Dialog.CurrentNode);
				link.l1.go = DialogGoNodeRepeat("exit", "", "", "", npchar, Dialog.CurrentNode);	
				break;
			}

		//редкостная хрень, но по-другому не работает-класс корабля ГГ считается отдельно от компаньонов, и всё тут
			int iClass, ipClass;
			ipClass = 4-sti(RealShips[sti(pchar.ship.type)].Class);
			iClass = 3;//т.к. не пройдет по числу кораблей в любом случае
		if (GetBaseHeroNation() == PIRATE)
		{
			ipClass = sti(ipClass)-1;
			int iChIdx, i;
			// поиск старшего класса компаньона
			for (i=0; i<COMPANION_MAX; i++)
			{
				iChIdx = GetCompanionIndex(GetMainCharacter(), i);
				if (iChIdx>=0)
				{
					sld = GetCharacter(iChIdx);
					iClass = GetCharacterShipClass(sld);
				}
			}
		}
			if (sti(ipClass) > 0 || 3 - sti(iClass) > 0)
			{
				dialog.text = NPCStringReactionRepeat("Ты бы еще на королевском мановаре явил"+ GetSexPhrase("ся","ась") +". Да твою посудину за милю видать из форта. Мы не будем рисковать своими головами. Приходи на меньшем корабле, и только на одном.", 
					"Я что, непонятно выражаюсь? Найди себе суденышко поменьше, тогда и приходи.", 
					"Ты что, тупица или притворяешься? Говорю же тебе - найди себе шхуну, ну бриг на крайний случай, или сделка не состоится.",
					"Ох, и как таких дур"+ GetSexPhrase("аков","") +" земля носит...", "block", 1, npchar, Dialog.CurrentNode);
				link.l1 = HeroStringReactionRepeat("Хорошо, я тебя понял"+ GetSexPhrase("","а") +". Приду позже, как кораблик сменю.", 
					"Да все понятно, просто уточнить хотел"+ GetSexPhrase("","а") +".",
					"Нет, не тупица, просто жадина. Подумал"+ GetSexPhrase("","а") +", может поменялось что. Я бы еще пару пинасов с собой прихватил"+ GetSexPhrase("","а") +"...", 
					"Ну ты же видишь - носит как-то...", npchar, Dialog.CurrentNode);
				link.l1.go = DialogGoNodeRepeat("exit", "", "", "", npchar, Dialog.CurrentNode);	
				break;
			}
			bOk  = CheckAttribute(pchar, "GenQuest.contraTravel.active") && (sti(pchar.GenQuest.contraTravel.active) == true);
			bOk2 = CheckAttribute(Pchar, "quest.Contraband.Active") && (sti(Pchar.quest.Contraband.Active) == true);
			bool bOk3 = CheckAttribute(Pchar, "questTemp.pirateStartQuest") && (Pchar.questTemp.pirateStartQuest == "3");
			if (bOk)
			{
			    if (GetQuestPastDayParam("contraTravel") > sti(PChar.GenQuest.contraTravel.days))
				{  // просрочка
					DeleteAttribute(PChar, "GenQuest.contraTravel");
					CloseQuestHeader("Gen_ContrabandTravel");
					bOk = false;
				}
			}

//navy --> PGG
			if (CheckFreeServiceForNPC(NPChar, "Smugglers") != -1)
			{
				Dialog.Text = "Извини, "+ GetSexPhrase("парень","деваха") +", у нас уже есть дела. Зайди через пару дней.";
				Link.l1 = "Жаль...";
				Link.l1.go = "Exit";		
				break;
			}
//navy <--

			if (bOk || bOk2 || bOk3)
			{
				Dialog.Text = "Может, стоит завершить одно дело прежде, чем браться за другое?";
				Link.l1 = "Пожалуй, ты прав.";
				Link.l1.go = "Exit";				
			}
			else
			{
                if (npchar.quest.trade_date != lastspeak_date)
    			{
                    npchar.quest.trade_date = lastspeak_date;
                    
                    if (ChangeContrabandRelation(pchar, 0) > 5)
                    {
                        Pchar.quest.contraband.CurrentPlace = SelectSmugglingLocation();
						Pchar.quest.contraband.City = NPChar.city;
                        if (Pchar.quest.contraband.CurrentPlace != "None")//boal fix
                        {
                            if (ChangeContrabandRelation(pchar, 0) >= 70)
                            {
                                Dialog.Text = "Я знаю, с тобой можно иметь дело. Мы будем ждать тебя в месте, называющемся " + GetConvertStr(Pchar.quest.contraband.CurrentPlace, "LocLables.txt") + ".";
                            }
                            else
                            {
            				    Dialog.Text = "Хм... Возможно, покупатель и найдется. Мы будем ждать вас в месте, называющемся " + GetConvertStr(Pchar.quest.contraband.CurrentPlace, "LocLables.txt") + ".";
            				}
            				Link.l1 = "Хорошо. До встречи.";
            				Link.l1.go = "Smuggling_exit";
        				}
        				else
        				{   //boal fix
                            Dialog.Text = "Сегодня сделок больше не будет. Приходи завтра.";
            				Link.l1 = "Хорошо.";
            				Link.l1.go = "Exit";
        				}
    				}
    				else
    				{
                        Dialog.Text = "И после всего ты думаешь, что кто-то захочет работать с тобой? Радуйся, что мы еще не послали наемных убийц за твоей головой. Убирайся!";
        				Link.l1 = "Эх... значит не судьба мне стать контрабандистом.";
        				Link.l1.go = "Exit";
    				}
				}
				else
				{
                    Dialog.Text = "Сегодня сделок больше не будет. Приходи завтра.";
    				Link.l1 = "Хорошо.";
    				Link.l1.go = "Exit";
				}
			}
		break;
////////////////////////////////////////////////////////////////////////////////
//	Корсарское метро
////////////////////////////////////////////////////////////////////////////////
		case "Travel":
//navy --> PGG
			if (CheckFreeServiceForNPC(NPChar, "Smugglers") != -1)
			{
				Dialog.Text = "Извини, "+ GetSexPhrase("парень","деваха") +", у нас уже есть дела. Зайди через пару дней.";
				Link.l1 = "Жаль...";
				Link.l1.go = "Exit";		
				break;
			}
//navy <--
			//если нет корабля у ГГ и нет компаньонов все ок
			if (sti(pchar.ship.type) == SHIP_NOTUSED && GetCompanionQuantity(pchar) == 1 && GetPassengersQuantity(pchar) == 0)
			{
				//случай если уже была инфа
				if (CheckAttribute(pchar, "GenQuest.contraTravel.active") && sti(pchar.GenQuest.contraTravel.active) == true)
				{
					//платил уже
					if (CheckAttribute(pchar, "GenQuest.contraTravel.payed") && sti(pchar.GenQuest.contraTravel.payed) == true)
					{
						Dialog.Text = "Мы, кажется уже договорились?";
						Link.l2 = "Да, точно!";
					}
					//не платил, значит можно запалатить пока не вышел срок.
					else
					{
						if(GetQuestPastDayParam("contraTravel") == sti(PChar.GenQuest.contraTravel.days))
						{
							Dialog.Text = "Принес"+ GetSexPhrase("","ла") +" деньги?";
							Link.l1 = "Да.";
							Link.l1.go = "Travel_pay";
							Link.l3 = "Я передумал"+ GetSexPhrase("","а") +"....";
							Link.l3.go = "Travel_abort";
							Link.l2 = "Нет пока.";
						}
						else
						{
                            if (GetQuestPastDayParam("contraTravel") < sti(PChar.GenQuest.contraTravel.days))
							{
								Dialog.Text = "Я тебе уже все сказал.";
								Link.l2 = "Точно.";
								Link.l1 = "Я передумал"+ GetSexPhrase("","а") +"....";
								Link.l1.go = "Travel_abort";
							}
							else // просрочка
							{
							    Dialog.Text = "Сегодня я ничем не могу помочь. Приходи через пару дней, может, что и будет.";
								Link.l2 = "Жаль.";
								DeleteAttribute(PChar, "GenQuest.contraTravel");
								CloseQuestHeader("Gen_ContrabandTravel");
							}
						}
					}
				}
				//если не было договора, обговариваем условия
				else
				{
					nRel = ChangeContrabandRelation(pchar, 0);
					//если нормальные отношения и количество подстав меньше 20, работаем....
					if (nRel > 0 && Statistic_AddValue(PChar, "contr_TravelKill", 0) < 20)
					{
						//бухта...
						pchar.GenQuest.contraTravel.CurrentPlace = SelectSmugglingLocation();
						aref arTmp; makearef(arTmp, pchar.GenQuest.contraTravel);
						SetSmugglersTravelDestination(arTmp);
						//за сколько доставят 
						pchar.GenQuest.contraTravel.price = (sti(PChar.rank)*250 + (100 - nRel)*10 + rand(30)*20) + sti(arTmp.destination.days)*100;
						
						//если метро активно, и нет пассажиров у ГГ, и еще сегодня не виделись, есть доступная бухта, и ранд ...
						bOk = !bPauseContrabandMetro && CheckNPCQuestDate(npchar, "Travel_Talk") && 
							Pchar.GenQuest.contraTravel.CurrentPlace != "None" && rand(50) < nRel;
                        bOk2 = CheckAttribute(Pchar, "quest.Contraband.Active") && (sti(Pchar.quest.Contraband.Active) == true);
						if (bOk && !bOk2)
						{
							nDay = 1 + rand(3);
							SetNPCQuestDate(npchar, "Travel_Talk");
							SaveCurrentQuestDateParam("contraTravel");

							Dialog.Text = "Что ж, можем доставить тебя до места " + GetConvertStr(locations[FindLocation(pchar.GenQuest.contraTravel.destination.loc)].id, "LocLables.txt") + " близ " +
								XI_ConvertString("Colony" + pchar.GenQuest.contraTravel.destination + "Gen") + " за " + pchar.GenQuest.contraTravel.price + " золотых. Принесешь деньги через " +
								FindRussianDaysString(nDay) + ". Корабль будет ждать тебя в месте под названием " +
								GetConvertStr(locations[FindLocation(Pchar.GenQuest.contraTravel.CurrentPlace)].id, "LocLables.txt") + " ровно сутки.";

							pchar.GenQuest.contraTravel.days = nDay;
							Link.l1 = "Хорошо, меня устраивает.";
							Link.l1.go = "Travel_agree";
							Link.l2 = "Нет, спасибо.";
						}
						else
						{
							Dialog.Text = "Сегодня я ничем не могу помочь. Приходи через пару дней, может что и будет.";
							Link.l2 = "Жаль.";
						}
					}
					//нет, посылаем в сад
					else
					{
                        Dialog.Text = "И после всего ты думаешь, что кто-то захочет работать с тобой? Радуйся, что мы еще не послали наемных убийц за твоей головой. Убирайся!";
        				Link.l2 = "Эх... значит не судьба.";
					}

				}
			}
			//корабль есть, посылаем в сад...
			else 
			{
				if(GetPassengersQuantity(pchar) != 0)
				{
					Dialog.Text = "Нет, мы не повезем вас. Только одного.";
					Link.l2 = RandSwear() + " И не нужно!";
				}
				else
				{
					Dialog.Text = "А свой корабль тебе зачем? Нет, мы этим не занимаемся.";
					Link.l2 = "Видно, не судьба...";
				}
			}
			Link.l2.go = "Exit";
			break;

		//отмена
		case "Travel_abort":
			ChangeContrabandRelation(pchar, -2);
			DeleteAttribute(PChar, "GenQuest.contraTravel");
			CloseQuestHeader("Gen_ContrabandTravel");
			Dialog.Text = "Ну, как хочешь...";
			Link.l1 = "";
			Link.l1.go = "Exit";
			break;

		//ГГ согласен ехать
		case "Travel_agree":
			ReOpenQuestHeader("Gen_ContrabandTravel");
			AddQuestRecord("Gen_ContrabandTravel", "1");
			AddQuestUserData("Gen_ContrabandTravel", "sSex", GetSexPhrase("","а"));
			AddQuestUserData("Gen_ContrabandTravel", "sLoc", GetConvertStr(pchar.GenQuest.contraTravel.CurrentPlace, "LocLables.txt"));
			AddQuestUserData("Gen_ContrabandTravel", "sLocTo", GetConvertStr(pchar.GenQuest.contraTravel.destination.loc, "LocLables.txt"));
			AddQuestUserData("Gen_ContrabandTravel", "sPlaceTo", XI_ConvertString("Colony" + pchar.GenQuest.contraTravel.destination + "Gen"));
			AddQuestUserData("Gen_ContrabandTravel", "sDays", FindRussianDaysString(sti(pchar.GenQuest.contraTravel.days)));
			AddQuestUserData("Gen_ContrabandTravel", "sPrice", pchar.GenQuest.contraTravel.price);

			//активируем квест
			pchar.GenQuest.contraTravel.active = true;
			NextDiag.CurrentNode = NextDiag.TempNode;
			DialogExit();
			break;

		//ГГ согласен платить
		case "Travel_pay":
			//денег хватает?
			Sum = sti(pchar.GenQuest.contraTravel.price);
			if (sti(pchar.money) >= Sum)
			{
				AddMoneyToCharacter(pchar, -1*Sum);
				//ставим флаг оплаты
				pchar.GenQuest.contraTravel.payed = true;
				Dialog.Text = "Приятно иметь с тобой дело. Корабль ждет, не опоздай.";
				Link.l1 = "Постараюсь.";
				AddQuestRecord("Gen_ContrabandTravel", "2");
				AddQuestUserData("Gen_ContrabandTravel", "sSex", GetSexPhrase("","а"));
				
				//ставим контру.
				PlaceSmugglersOnShore(PChar.GenQuest.contraTravel.CurrentPlace);
				//корабль на волнах в бухте....
				Sum = sti(pchar.GenQuest.contraTravel.destination.days);
				sld = GetCharacter(NPC_GenerateCharacter("Abracham_Gray", "pirate_1", "man", "man", 5, PIRATE, Sum + 2, true, F_SMUGGLER));
				//воскресим...
				sld.nation = PIRATE;
				SetRandomNameToCharacter(sld);
				SetMerchantShip(sld, rand(GOOD_SILVER));
				SetFantomParamHunter(sld);
				SetCaptanModelByEncType(sld, "pirate");
				SetCharacterShipLocation(sld, PChar.GenQuest.contraTravel.CurrentPlace);

				SetTimerCondition("RemoveTravelSmugglers", 0, 0, 1, false);
			}
			//нет, посылаем в сад...
			else
			{
				Dialog.Text = "Похоже, у тебя проблемы с наличностью."
				Link.l1 = "Жаль, приду позже.";
			}
			Link.l1.go = "Exit";
			break;  	
////////////////////////////////////////////////////////////////////////////////
//	END OF Корсарское метро
////////////////////////////////////////////////////////////////////////////////
	}
}
