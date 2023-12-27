void ProcessDialogEvent()
{
	ref NPChar, sld, location;
	aref Link, NextDiag;
	string sTemp,sTemp1, str, str1;
	int	s1,s2,s3,s4,s5,p1,iColony;

	DeleteAttribute(&Dialog,"Links");

	makeref(NPChar,CharacterRef);
	makearef(Link, Dialog.Links);
	makearef(NextDiag, NPChar.Dialog);
	
    // вызов диалога по городам -->
    NPChar.FileDialog2 = "DIALOGS\" + LanguageGetLanguage() + "\Brothel\" + NPChar.City + "_Brothel.c";
    if (LoadSegment(NPChar.FileDialog2))
	{
        ProcessCommonDialog(NPChar, Link, NextDiag);
		UnloadSegment(NPChar.FileDialog2);
	}
    // вызов диалога по городам <--
	switch(Dialog.CurrentNode)
	{
		// ============= хозяйка борделя =============
		case "First time":
			if (LAi_grp_playeralarm > 0)
			{
       			dialog.text = NPCharRepPhrase(pchar, 
					LinkRandPhrase(""+ GetSexPhrase("Милый-дорогой, какие девочки?! За тобой половина гарнизона гонится, а он в бордель пожаловал!","Шла бы ты отсюда... За тобой половина гарнизона гонится!") +"", "Вся городская стража рыщет по городу в поисках тебя. Я не идиотка, привечать тебя в такой момент...", "Попрошу удалиться из моего заведения, тебе здесь делать нечего!"), 
					LinkRandPhrase(""+ GetSexPhrase("Попробуй только дотронуться до моих девочек - я с тебя живого шкуру сдеру!","Убирайся отсюда, мерзавка!") +"", "Грязн"+ GetSexPhrase("ый","ая") +" убийца, вон из моего заведения! Стража!!", "Я не боюсь тебя, мерзав"+ GetSexPhrase("ец","ка") +"! Скоро тебя повесят в нашем форте, далеко тебе не уйти..."));
				link.l1 = NPCharRepPhrase(pchar,
					RandPhraseSimple("Хех, тревога для меня не проблема...", "Им меня ни за что не поймать."), 
					RandPhraseSimple("Помолчала бы лучше, дура...", "Заткнись, не то хуже будет..."));
				link.l1.go = "exit";
				break;
			}
			
			if (npchar.quest.meeting == "0")
			{
				dialog.text = RandPhraseSimple(TimeGreeting() + ". Добро пожаловать в дом терпимости. Меня зовут " + npchar.name + ", я здесь заведую всем. "+ GetSexPhrase("Чем могу быть полезна?, " + GetAddress_Form(NPChar) + "","Признаюсь, немного удивлена видеть вас здесь, " + GetAddress_Form(NPChar) + ", но смею заверить: мы оказываем услуги не только мужчинам.") +"",
				                               TimeGreeting() + ". Приветствую вас, "+ GetSexPhrase("незнакомец","девушка") +", в моем скромном заведении. Позвольте представится, я "+ NPChar.Name + " - владелица этого приюта обиженных лаской мужчин. "+ GetSexPhrase("Чем я могу быть вам полезна?","Хотя для женщин у нас тоже кое-что есть...") +"");
				npchar.quest.meeting = "1";
			}
			else
			{
				dialog.text = RandPhraseSimple(TimeGreeting() + ". Чем я могу вам помочь, " + GetAddress_Form(NPChar) + "?",
				                               TimeGreeting() + ". Что я могу для вас сделать, " + GetAddress_Form(NPChar) + "?");
			}
			link.l2 = npchar.name + ", я хочу провести время с одной из твоих девушек.";
			link.l2.go = "Hostess_1";
			link.l3 = "Давно я не баловал"+ GetSexPhrase("","а") +" своих парней. Можно сделать оптовый заказ на девочек?";
			link.l3.go = "ForCrew";
			link.l4 = "У меня есть вопрос.";
            link.l4.go = "quests";	
			link.l9 = "Ничем. Я уже ухожу.";
			link.l9.go = "exit";
			NextDiag.TempNode = "First time";
		break;
		
        case "Hostess_1":
			if (!CheckAttribute(npchar, "quest.selected"))
			{
				if (CheckNPCQuestDate(npchar, "quest.date"))
				{
					dialog.text = ""+ GetSexPhrase("Мы всегда рады клиенту. Скажи мне, дорогой, ты уже присмотрел кого-то или тебе все равно?","Ну что же, мои девочки вполне смогут тебе... помочь. Ты присмотрела кого-нибудь, или тебе все равно?") +"";
					Link.l1 = ""+ GetSexPhrase("Хех, мне нужна девка и немедленно, а кто она такая - наплевать. У тебя все они симпатичные...","Да любая, лишь бы дело свое знала...") +"";
					Link.l1.go = "Hostess_NotChoice";	
					Link.l2 = "Да, есть одна, "+ GetSexPhrase("что мне приглянулась...","с ней было бы приятнее...") +"";
					Link.l2.go = "Hostess_Choice";
				}
				else
				{
					dialog.text = "Сегодня я уже не могу дать тебе девочку, кроме тебя у меня еще есть клиенты. Приходи завтра, обслужим как следует!";
					Link.l1 = "Эх, жаль"+ GetSexPhrase(", я только вошел во вкус","") +"... Ну да ладно.";
					Link.l1.go = "exit";
				}
			}
			else
			{
				dialog.text = "Хм, ты уже оплатил"+ GetSexPhrase("","а") +" девушку. Так что займись ею вплотную, так сказать, и не отвлекай меня по пустякам.";
				Link.l1 = "Хорошо, уже иду.";
				Link.l1.go = "exit";	
			}
		break;

        case "Hostess_NotChoice":
			sld = GetFreeHorseRef(npchar.city);
			location = &locations[FindLocation(npchar.city + "_Brothel")];
			if (sld.id == "none" || GetNpcQuestPastDayParam(location, "Brothel_date") > 98)
			{
				dialog.text = "Сейчас у меня нет свободных девушек, тебе нужно будет зайти сюда через пару дней.";
				Link.l1 = "Хорошо, как скажешь.";
				Link.l1.go = "exit";	
			}
			else
			{
				dialog.text = ""+ GetSexPhrase("Ну что, жеребец, превосходно!","У меня они все умелые, можешь не сомневаться.") +" Могу предложить прекрасную девушку по имени " + GetFullName(sld) + ", она сейчас свободна. Стоить это удовольствие будет " + FindRussianMoneyString(sti(sld.quest.price)) + ". Соглас"+ GetSexPhrase("ен","на") +"?";
				Link.l1 = "Нет, пожалуй, откажусь. Дороговато...";
				Link.l1.go = "exit";
				if (sti(pchar.money) >= sti(sld.quest.price))
				{
					Link.l2 = "Конечно, соглас"+ GetSexPhrase("ен","на") +", какие могут быть вопросы?!";
					Link.l2.go = "Hostess_NotChoice_agree";	
					npchar.quest.choiceIdx = sld.index;
				}
				else
				{
					Link.l1 = "Агрх, нет у меня сейчас таких денег...";
					Link.l1.go = "exit";
				}
			}
		break;
        case "Hostess_NotChoice_agree":
			sld = &characters[sti(npchar.quest.choiceIdx)];
			if (sti(pchar.money) >= sti(sld.quest.price))
			{
				dialog.text = "Отлично, дорог"+ GetSexPhrase("ой","уша") +". " + sld.name + " будет ждать тебя в комнате для уединения на втором этаже.";
				Link.l1 = ""+ GetSexPhrase("Хех, ну я пошел","Ну, я побежала") +"...";
				Link.l1.go = "exit";
				AddMoneyToCharacter(pchar, -sti(sld.quest.price));
				sld.dialog.currentnode = "Horse_ReadyFack";			
				//--> таймер на возврат, чтобы не вечно ждали
				str = npchar.city;
				pchar.quest.(str).win_condition.l1            = "Timer";
				pchar.quest.(str).win_condition.l1.date.day   = GetAddingDataDay(0, 0, 1);
				pchar.quest.(str).win_condition.l1.date.month = GetAddingDataMonth(0, 0, 1);
				pchar.quest.(str).win_condition.l1.date.year  = GetAddingDataYear(0, 0, 1);
				pchar.quest.(str).win_condition               = "Brothel_checkVisitTime";	
				pchar.quest.(str).HorseId = sld.id;
				pchar.quest.(str).locator = sld.location.locator;
				//<-- таймер на возврат, чтобы не вечно ждали
				ChangeCharacterAddressGroup(sld, npchar.city + "_Brothel_room", "goto", "goto"+(rand(2)+1));
				LocatorReloadEnterDisable(npchar.city + "_Brothel", "reload2_back", false); //открываем комнату
				npchar.quest.selected = true; //флаг взял девку у хозяйки
				SetNPCQuestDate(npchar, "quest.date"); //дату взятия запишем
				for(int n=0; n<MAX_CHARACTERS; n++)
				{
					makeref(sld, Characters[n]);
					if (sld.location == npchar.city+"_Brothel" && CheckAttribute(sld, "CityType") && sld.CityType == "horse")
					{
						sld.dialog.currentnode = "Horse_4";
					}
				}
			}
			else
			{
				dialog.text = "Все бы было хорошо, но дело в том, что " + sld.name + " - девушка недешевая, стоит " + FindRussianMoneyString(sti(sld.quest.price)) + ". А у тебя, насколько я вижу, таких денег нет. Приходи как разбогатеешь"+ GetSexPhrase(", родной","") +"...";
				Link.l1 = "Эх, вот так всегда...";
				Link.l1.go = "exit";
			}
		break;

        case "Hostess_Choice":
			dialog.text = "Я всегда рада, когда между девочками и клиентами возникают теплые чувства... Назови мне ее имя.";
			Link.l1.edit = 9;
			Link.l1 = "";
			Link.l1.go = "Hostess_Choice_1";	
		break;
        case "Hostess_Choice_1":
			sld = CheckHorsesName(npchar.city, 9);
			if (sld.id == "none")
			{
				dialog.text = "Хм, ты ошибаешься, у меня нет такой девочки в заведении. Возможно, ты неверно называешь ее имя.";
				Link.l1 = "Хм, но я только что с ней разговаривал"+ GetSexPhrase("","а") +".";
				Link.l1.go = "Hostess_Choice_2";				
				Link.l2 = "Пожалуй, мне лучше уточнить ее имя. Я позже переговорю с тобой на эту тему.";
				Link.l2.go = "exit";	
			}
			else
			{
				dialog.text = GetFullName(sld) + ", ты о ней говоришь?";
				Link.l1 = "Ага, именно о ней.";
				Link.l1.go = "Hostess_NotChoice_agree";				
				Link.l2 = "Нет, не о ней.";
				Link.l2.go = "Hostess_Choice_2";
				npchar.quest.choiceIdx = sld.index;
			}
		break;
        case "Hostess_Choice_2":
			dialog.text = "Тогда тебе нужно назвать ее имя еще раз, возможно я пойму, о ком идет речь.";
			Link.l1.edit = 9;
			Link.l1 = "";
			Link.l1.go = "Hostess_Choice_1";
			Link.l2 = "Пожалуй, мне лучше уточнить ее имя. Я позже переговорю с тобой на эту тему.";
			Link.l2.go = "exit";	
		break;
		//==> команда
		case "ForCrew":
			dialog.text = "Хмм.. На всю команду? Значит, решил"+ GetSexPhrase("","а") +" 'сбросить пар' у ребят? Что ж, у меня есть несколько высоко квалифицированных девочек, способных на такое... С тебя " + FindRussianMoneyString(GetCrewQuantity(pchar)*30) + ".";
			link.l1 = "Хорошо, я соглас"+ GetSexPhrase("ен","на") +" заплатить эти деньги.";
			link.l1.go = "ForCrew_1";
			link.l2 = "Думаю, они обойдутся как-нибудь...";
			link.l2.go = "exit";
		break;
		
		case "ForCrew_1":
		    if (sti(Pchar.money) >= GetCrewQuantity(pchar)*30 && GetCrewQuantity(pchar)>0)
		    {
		        AddMoneyToCharacter(Pchar, -makeint(GetCrewQuantity(pchar)*30));

	            AddCrewMorale(Pchar, 10);
	            LAi_Fade("", "");
                AddTimeToCurrent(5 + rand(1), rand(30));
			    DialogExit();
		    }
		    else
		    {
		        dialog.text = "Это, конечно, не мое дело, но мне кажется, что тебе сначала нужно заработать на команду, а потом думать о ее морали.";
			    link.l1 = "Пожалуй, да...";
			    link.l1.go = "exit";
		    }
		break;
		case "Woman_FackYou":
			dialog.text = "Мил"+ GetSexPhrase("ый","ая") +", ты куда это полез"+ GetSexPhrase("","ла") +"?! Надо же, а казался порядочн"+ GetSexPhrase("ым капитаном","ой девушкой") +".\nНу а теперь так просто тебе не уйти, красав"+ GetSexPhrase("чик","ица") +". Подрежут тебе крылышки...";
			link.l1 = "Заткнись, дура...";
			link.l1.go = "exit_setOwner";
			LAi_group_Attack(NPChar, Pchar);
			if (rand(3) != 1) SetNationRelation2MainCharacter(sti(npchar.nation), RELATION_ENEMY);
		break;
		
		// ================================== рядовой состав =======================================
        case "Horse_talk":
			if (LAi_grp_playeralarm > 0)
			{
       			dialog.text = NPCharRepPhrase(pchar, 
					LinkRandPhrase("Вам лучше уйти из заведения!", "Вся городская стража рыщет по городу! Вам лучше уйти...", "Надебоширили - и к нам?! Нет уж, в другой раз..."), 
					LinkRandPhrase("Убирайся!!", "Грязн"+ GetSexPhrase("ый","ая") +" убийца, вон отсюда! Стража!!", "Я не боюсь тебя, мерзав"+ GetSexPhrase("ец","ка") +"! Скоро тебя повесят в нашем форте, далеко тебе не уйти..."));
				link.l1 = NPCharRepPhrase(pchar,
					RandPhraseSimple("Хех, тревога для меня не проблема...", "Им меня ни за что не поймать."), 
					RandPhraseSimple("Хех, ну и дура же ты...", "Заткнись, не то хуже будет..."));
				link.l1.go = "exit";
				break;
			}
			dialog.text = NPCStringReactionRepeat("Здравствуйте, " + GetAddress_Form(NPChar) + ""+ GetSexPhrase("",", хи-хи-хи..") +". Вам нужно обратиться к хозяйке заведения, все заказы проходят через нее.", 
				"Ой, это опять вы. Простите, но вам нужно сначала уладить все вопросы с хозяйкой. Прошу вас пройти к ней.", 
				"Послушайте, " + GetAddress_Form(NPChar) + ""+ GetSexPhrase(", я очень надеюсь, что вы так же упрямы и в других делах, а не только в разговорах... Еще","я еще") +" раз прошу вас пройти к хозяйке заведения.", 
				"Ах, " + GetAddress_Form(NPChar) + ""+ GetSexPhrase(", какой же вы... настойчивый!",", хи-хи-хи... Вы, однако, натойчивы.") +" Вам нужно пройти к хозяйке заведения для оформления заказа.", "block", 1, npchar, Dialog.CurrentNode);
			link.l1 = HeroStringReactionRepeat("Хорошо, "+ GetSexPhrase("красавица, ","") +"я понял"+ GetSexPhrase("","а") +".", "А-а, да, конечно...",
                      ""+ GetSexPhrase("Можешь не сомневаться, красавица, я упрям и силен, как бык!","Да-да...") +"", ""+ GetSexPhrase("Вот дьявол, что-то упустил... Прости, милая.","Хорошо, хорошо.") +"", npchar, Dialog.CurrentNode);
			link.l1.go = "exit";
			Link.l2 = "А где она находится?";
			Link.l2.go = "Horse_1";
			Link.l3 = LinkRandPhrase(""+ GetSexPhrase("Аргх, красавица, в твоих глазах можно утонуть...","Ты выглядишь просто замечательно, малышка!") +"", ""+ GetSexPhrase("Знаешь, я давно не видел такой красивой женщины!","Знаешь, я давненько не встречала такой симпатяшки!") +"", ""+ GetSexPhrase("Эх, милая, ты невероятно прелестна.","Эх, надоели все эти неотесаные мужланы... А ты просто прелесть!") +"");
			if (!CheckAttribute(npchar, "quest.choice"))
			{
				Link.l3.go = "Horse_2";
			}
			else
			{
				Link.l3.go = "HorseChoice_" + npchar.quest.choice;
			}
			NextDiag.TempNode = "Horse_talk";
		break;
        case "Horse_1":
			dialog.text = "Она в своем кабинете, вы можете пройти к ней отсюда через дверь, противоположную выходу на улицу, либо через улицу с другой стороны дома. Ее зовут " + characters[GetCharacterIndex(npchar.city + "_Hostess")].name + ".";
			Link.l1 = "Понятно, "+ GetSexPhrase("милая","дорогуша") +", спасибо.";
			Link.l1.go = "exit";			
		break;
        case "Horse_2":
			if (rand(1))
			{
				dialog.text = LinkRandPhrase("Ах, боже мой, до чего приятно слышать такое! Слушай, я сейчас свободна, так что если ты выберешь меня, то не пожалеешь...", "Ты действительно так считаешь? Мне очень и очень приятно... Послушай, я сейчас свободна, так что ты можешь выбрать меня."+ GetSexPhrase(" Я обещаю тебе море любви и океан ласки...","") +"", ""+ GetSexPhrase("Вот как?! Хм, не скрою, я польщена, не часто приходится слышать в свой адрес такие слова...","Эх, девушка... как меня достали эти мужики...") +" Слушай, я не занята в данный момент, так что я предлагаю тебе выбрать меня. Обещаю, что ты останешься довол"+ GetSexPhrase("ен","ьна") +"...");
				link.l1 = "Хех, именно тебя и выбираю!";
				Link.l1.go = "Horse_3";		
				Link.l2 = ""+ GetSexPhrase("Не-е-ет, это был просто комплимент прекрасной даме","Это был просто комплимент") +".";
				Link.l2.go = "exit";
				npchar.quest.choice = 0; //был базар, но ГГ потом отказался
			}
			else
			{
				dialog.text = LinkRandPhrase(""+ GetSexPhrase("Знаешь что я тебе скажу, милый мой? Не нужно мне пустых разговоров. Займись делом!","Ну вот только этого мне не хватало - комплиментов от женщин!") +"", ""+ GetSexPhrase("Ты тоже воображаешь, что все женщины любят ушами? Хм, это очень зря... Если хочешь меня - плати хозяйке заведения, а трепотня мне не нужна.","Девушка, не растрачивайтесь на пустые слова. Хотите дел - платите...") +"", ""+ GetSexPhrase("Еще один любитель чувственности... Плати - и я буду твоей, но только без этого нежного бреда!","Ты чего, дорогуша? Хочешь удовольствия - плати, а не занимайся болтовней!") +"");
				link.l1 = "Ого, какая хватка!";
				Link.l1.go = "exit";
				npchar.quest.choice = 2; //ГГ послали
			}
		break;
        case "Horse_3":
			dialog.text = characters[GetCharacterIndex(npchar.city + "_Hostess")].name + " все оформляет в своем кабинете. Иди к ней"+ GetSexPhrase(", мой герой,","") +" и назови мое имя - " + npchar.name + ". Я буду тебя ждать...";
			Link.l1 = "Понятно, милая, скоро буду...";
			Link.l1.go = "exit";
			npchar.quest.choice = 1; //она согласная
			SetNPCQuestDate(npchar, "quest.choice");
		break;
        case "Horse_4": 
			dialog.text = NPCStringReactionRepeat("Ты уже оплатил"+ GetSexPhrase("","а") +" девушку, она ждет тебя, поднимайся наверх.", 
				"Я же тебе сказала - поднимайся к ней.", 
				"Слушай внимательно, еще раз повторяю - поднимайся к ней...", 
				"Бесполезно говорить, не доходит...", "block", 1, npchar, Dialog.CurrentNode);
			link.l1 = HeroStringReactionRepeat("Да, я знаю.", "Я помню.",
                      "Можешь не повторять, о ней я помню.", "Хм, о чем это ты?..", npchar, Dialog.CurrentNode);
			link.l1.go = "exit";
			NextDiag.TempNode = "Horse_4";
		break;
		//===>> реакция на попытки пофлиртовыть, если флирт уже был
        case "HorseChoice_0": 
			if (!CheckAttribute(npchar, "quest.sexHappend"))
			{
				dialog.text = NPCStringReactionRepeat("Что-то я тебя не пойму."+ GetSexPhrase(" То комплименты делаешь, то вдруг на попятную идешь. Странный тип...","") +"", 
					"Опять со своим комплиментами? Хм, иди к хозяйке, с ней все решай.", 
					"Хм, и не надоело тебе? Сказала же - иди к хозяйке заведения.", 
					"Нам нельзя здесь оскорблять клиентов, но ты нарываешься, мил"+ GetSexPhrase("ый","ая") +"...", "block", 1, npchar, Dialog.CurrentNode);
				link.l1 = HeroStringReactionRepeat("Так вышло, знаешь ли...", "Хорошо, я так и сделаю.",
						"Да-да, я понял"+ GetSexPhrase("","а") +"...", "Хм, извини, дорогуша.", npchar, Dialog.CurrentNode);
				link.l1.go = "exit";
			}
			else
			{
				dialog.text = "Спасибо за комплимент. Если ты хочешь взять меня, то иди к хозяйке заведения. Все как всегда...";
				Link.l1 = "Понятно.";
				Link.l1.go = "exit";
			}
		break;
        case "HorseChoice_1": 
			if (!CheckAttribute(npchar, "quest.sexHappend"))
			{
				if (!CheckNPCQuestDate(npchar, "quest.choice"))
				{
					dialog.text = NPCStringReactionRepeat(""+ GetSexPhrase("Милый, мы все оговорили. ","") +"Не заставляй меня долго ждать...", 
						""+ GetSexPhrase("Хм, послушай, милый, мне","Мне") +" очень приятны все твои слова, но пора уже и за дело браться...", 
						"Послушай, может ты все-таки уладишь все с хозяйкой?..", 
						"Хм, даже не знаю, что и сказать...", "block", 1, npchar, Dialog.CurrentNode);
					link.l1 = HeroStringReactionRepeat("Да ни за что на свете!", "Обязательно!",
							"Да, конечно!", "Бегу, бегу к хозяйке...", npchar, Dialog.CurrentNode);
					link.l1.go = "exit";
				}
				else
				{
					dialog.text = "Я тебя ждала, а ты так и не взял"+ GetSexPhrase("","а") +" меня! Не могу сказать, что я в восторге от этого...";
					Link.l1 = "Гм, так получилось...";
					Link.l1.go = "exit";
					npchar.quest.choice = 0; //был базар, но ГГ потом отказался
				}
			}
			else
			{
				dialog.text = "Ах-х, это опять ты, мо"+ GetSexPhrase("й славный корсар","я славная девушка") +"! Если тебе опять нужна я, то иди к хозяйке - не пожалеешь..."+ GetSexPhrase(" Кстати, я тебя узнала, дорогой, только мы вынуждены обращаться ко всем клиентам стандартно, ты уж извини...","") +"";
				Link.l1 = "Подожди меня совсем чуть-чуть, "+ GetSexPhrase("красавица","милашка") +", и мы снова с тобой уединимся.";
				Link.l1.go = "exit";
				Link.l2 = "Мне очень приятно, что ты меня запомнила, но сейчас провести с тобой ночь я, сожалению, не могу.";
				Link.l2.go = "HorseChoice_1_Add";
			}
		break;
        case "HorseChoice_2": 
			if (!CheckAttribute(npchar, "quest.sexHappend"))
			{
				dialog.text = NPCStringReactionRepeat("Вот что,"+ GetSexPhrase(" милый друг,","") +" займись-ка лучше делом. Это полезней будет, чем языком чесать.", 
					""+ GetSexPhrase("Хм, странный ты,","Ну что ты") +" талдычишь одно и тоже...", 
					"Послушай, может хватит уже?!", 
					"Хм, ну надо же, ничего нового"+ GetSexPhrase(", опять глупые попытки очаровать. Хочешь со мной переспать - иди к хозяйке заведения, недоумок!","...") +"", "block", 1, npchar, Dialog.CurrentNode);
				link.l1 = HeroStringReactionRepeat("Да, слышал"+ GetSexPhrase("","а") +"...", "Хм, вот так оно и выходит...",
						"Хм, может хватит, а может и нет...", "Осторожней в выражениях"+ GetSexPhrase(", коза","") +"...", npchar, Dialog.CurrentNode);
				link.l1.go = "exit";
			}
			else
			{
				dialog.text = "А-а-а, это опять ты! "+ GetSexPhrase("И снова с глупыми хвалебными речами, ничего не меняется в этом мире... Е","Напоминаю, е") +"сли хочешь "+ GetSexPhrase("переспать","побаловаться") +" со мной - плати хозяйке заведения, а слова твои мне безразличны...";
				Link.l1 = "Хех, кто бы сомневался...";
				Link.l1.go = "exit";
			}
		break;
        case "HorseChoice_1_Add":
			dialog.text = "Хм, не скажу, что меня это обрадовало... Очень жаль.";
			Link.l1 = "Извини...";
			Link.l1.go = "exit";
			npchar.quest.choice = 0;
		break;
		//===>> секс
        case "Horse_ReadyFack":
			if (!CheckAttribute(npchar, "quest.choice")) npchar.quest.choice = 0;
			switch(npchar.quest.choice)
			{
				case "0":
					dialog.text = RandPhraseSimple("Очень рада видеть тебя в комнате уединения. "+ GetSexPhrase("И чем мы дальше займемся?","Ну что же, будем развлекаться?") +"", "Проходи, не стесняйся"+ GetSexPhrase(", чувствуй себя как дома на ближайшие два часа.",". Я сумею тебя... развлечь, не переживай.") +"");
					Link.l1 = RandPhraseSimple("Скучать не будем, полагаю...", ""+ GetSexPhrase("Давай повеселимся, детка!","Не будем терять времени!") +"");
				break;
				case "1":
					dialog.text = "Ах, вот и ты, мо"+ GetSexPhrase("й славный корсар! Я кое-что обещала тебе и готова сдержать свое слово","я славная девушка! Следующие два часа ты не забудешь никогда") +"...";
					Link.l1 = "Ух, как заманчиво звучит...";	
				break;
				case "2":
					dialog.text = "А-а-а, приш"+ GetSexPhrase("ел","ла") +", наконец-то. Ну, не будем терять времени!";
					Link.l1 = ""+ GetSexPhrase("Не будем, киска...","Ну, покажи мне, что ты можешь...") +"";
				break;
			}
			Link.l1.go = "exit";
			str = npchar.city;
			pchar.quest.(str).win_condition.l1 = "ExitFromLocation";
			pchar.quest.(str).win_condition.l1.location = pchar.location;
			pchar.quest.(str).win_condition = "Brothel_checkVisitTime";
			pchar.questTemp.HorseQty = sti(pchar.questTemp.HorseQty) + 1; //счетчик
			NextDiag.TempNode = "Horse_AfterSex";
			AddDialogExitQuest("PlaySex_1");
			
			/// кач от борделя
			if (CheckNPCQuestDate(pchar, "BrothelSex"))
			{
				AddCharacterHealth(pchar, 5);
				SetNPCQuestDate(pchar, "BrothelSex");
			}
			// изменение статусов
			ChangeCharacterReputation(pchar, -1);
			AddCharacterExpToSkill(pchar, "Leadership", 30);
			AddCharacterExpToSkill(pchar, "Fencing", -15);
			AddCharacterExpToSkill(pchar, "Pistol", -15);
		break;

        case "Horse_AfterSex":
			switch(npchar.quest.choice)
			{
				case "0":
					dialog.text = LinkRandPhrase("Тебе понравилось?", "Ну, что скажешь? Все в порядке?", "Как дела,"+ GetSexPhrase(" корсар, все"," все") +" нормально?");
					Link.l1 = RandPhraseSimple(""+ GetSexPhrase("Конечно, все в порядке","А ты и впрямь... умеешь") +".", ""+ GetSexPhrase("Все в норме, крошка","Ты знаешь, я вполне довольна") +".");
					Link.l1.go = "exit";
				break;
				
				case "1":
					dialog.text = LinkRandPhrase("Ну что, я выполнила обещание?", "Ну, как"+ GetSexPhrase(" я тебе, понравилась",", тебе понравилось") +"?", "Я надеюсь, "+ GetSexPhrase("ты доволен, потому что я о-о-очень довольна","ты довольна. Я старалась") +"...");
					Link.l1 = RandPhraseSimple("Да, мне все очень понравилось.", ""+ GetSexPhrase("Отлично покувыркались, ты была на высоте!","Все было просто замечательно!") +"");	
					Link.l1.go = "exit";
				break;
				
				case "2":
					dialog.text = RandPhraseSimple("Ну все, тебе пора.", "Время вышло,"+ GetSexPhrase(" корсар,","") +" тебе пора на выход.");
					Link.l1 = RandPhraseSimple("Ага, пока...", "До свидания и спасибо тебе...");
					Link.l1.go = "exit";
				break;
			}
			NextDiag.TempNode = "Horse_AfterSex_2";
		break;
		
        case "Horse_AfterSex_2":
            dialog.text = LinkRandPhrase("Заходи к нам еще как-нибудь...", "До свидания и до новых встреч...", "Будем рады видеть тебя у нас еще...");
			Link.l1 = "Ладно...";
			Link.l1.go = "exit";
			NextDiag.TempNode = "Horse_AfterSex_2";
		break;
		
		case "exit_setOwner":
			LAi_SetOwnerTypeNoGroup(npchar);
			NextDiag.CurrentNode = NextDiag.TempNode;
			DialogExit();
		break;
		
		case "Exit":
			NextDiag.CurrentNode = NextDiag.TempNode;
			DialogExit();
		break;
	}
}

ref GetFreeHorseRef(string City)
{
	ref rCharacter;
    int storeArray[8];
	int howStore = 0;
	for(int n=0; n<MAX_CHARACTERS; n++)
	{
		makeref(rCharacter, Characters[n]);
		if (rCharacter.location == City+"_Brothel" && CheckAttribute(rCharacter, "CityType") && rCharacter.CityType == "horse")
		{
			storeArray[howStore] = n;
			howStore++;
		}
	}
	if (howStore == 0) return &NullCharacter;
	return &characters[storeArray[rand(howStore-1)]];
}

ref CheckHorsesName(string City, int num)
{
	ref rCharacter;
	string sTemp, sSeeked;
	sSeeked = GetStrSmallRegister(dialogEditStrings[num]);
	for(int n=0; n<MAX_CHARACTERS; n++)
	{
		makeref(rCharacter, Characters[n]);
		if (rCharacter.location == City+"_Brothel" && CheckAttribute(rCharacter, "CityType") && rCharacter.CityType == "horse")
		{
			sTemp = GetStrSmallRegister(rCharacter.lastname + " " + rCharacter.name);
			if (findsubstr(sSeeked, sTemp , 0) != -1)
				return rCharacter;			
			sTemp = GetStrSmallRegister(rCharacter.lastname);
			if (findsubstr(sSeeked, sTemp , 0) != -1)
				return rCharacter;
			sTemp = GetStrSmallRegister(rCharacter.name);
			if (findsubstr(sSeeked, sTemp , 0) != -1)
				return rCharacter;
		}
	}
	return &NullCharacter;
}
