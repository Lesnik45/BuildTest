// Слухи - диалог по типажам

#define MAX_RUMOURS_ABOUT_OWNERS 7

void ProcessCommonDialogRumors(ref NPChar, aref Link, aref NextDiag);
{
    string strum;
    string srum;
	switch(Dialog.CurrentNode)
	{
	case "rumours_citizen":	
		NextDiag.CurrentNode = "rumours";
		//------------- квестовые слухи горожан --------->>>>>>>>
		
		//<<<<<-------- квестовые слухи горожан -----------------
		if (!CheckAttribute(NPChar, "quest.repeat.rumours_citizen") || NPChar.quest.repeat.rumours_citizen != 3 )
        srum = SelectRumourEx("citizen", NPChar);
        else srum = NO_RUMOUR_TEXT[rand(SIMPLE_RUMOUR_NUM - 1)]; // fix
        string posrep1, posrep2, answ1, answ2, answ3, answ4;
        if (RumourHasInformation(srum))
        {
            posrep1 = RandPhraseSimple(" Такие вот дела...", " Возможно, вас это позабавит.");
            posrep2 = " А у вас, капитан, есть какие-нибудь известия для нашей колонии?";
            answ1 = RandPhraseSimple(RandSwear() + "Интересные вещи ты рассказываешь, "+GetFullName(NPChar)+".",
                                 "Тогда расскажи мне еще...");
            answ2 = RandPhraseSimple(RandSwear() + "Это очень интересно! Вот что еще хотел"+ GetSexPhrase("","а") +" спросить...",
                                 "Еще вопрос.");
            answ3 = RandPhraseSimple("Чего только не случается на свете. Мне жаль, но никаких новостей у меня нет. ",
                                 "Я немного спешу, так что как-нибудь в другой раз.");
            answ4 = "";
        
        }
        else
        {
            posrep1 = " У нас вообще редко что-либо происходит. Но если же произойдет что-то необычное, то все об этом только и говорят.";
            posrep2 = " Так что новостей никаких. Но может " + GetAddress_Form(NPChar) + " капитан что-то знает?";
            answ1 = RandPhraseSimple("Тогда расскажи-ка мне лучше...",
                                 RandSwear() + "Ты ничего не знаешь! Ладно, вот что еще хотел"+ GetSexPhrase("","а") +" спросить...");
            answ2 = RandPhraseSimple("Говоришь, не знаешь, ладно, скажи мне тогда...",
                                 "Да, не сильно ты мне помог"+NPCharSexPhrase(NPChar, "","ла")+", но, может, ты знаешь что-то еще?");
            answ3 = RandPhraseSimple("Не думаю, что знаю что-то что, могло бы вас заинтересовать.",
                                 "Я с удовольствием что-нибудь расскажу, но как-нибудь в другой раз.");
            answ4 = "";
        
        }
		Dialog.Text = NPCStringReactionRepeat(srum,
                                                srum+posrep1,
                                                srum+posrep2,
                                                RandPhraseSimple("К сожалению, больше я ничего не знаю, позвольте мне идти.",
                                                                    "Вы утомили меня своими расспросами, прошу меня простить, но меня ждут дела."),
                                                "block", 1, npchar, Dialog.CurrentNode);
												
        link.l1 = HeroStringReactionRepeat(answ1,
                                            answ2,
                                            answ3,
                                            answ4,
                                            npchar, Dialog.CurrentNode);
		link.l1.go = "new question";
		link.l2 = RandPhraseSimple("Спасибо, долж"+ GetSexPhrase("ен","на") +" откланяться.",
                                 "Всего хорошего.");
		link.l2.go = "exit";
	break;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	case "rumours_habitue":
		Dialog.Text = LinkRandPhrase("Ну... И-ик! ","Эх, какой ром! ","Н-да... ")+SelectRumourEx("habitue", NPChar);
		link.l1 = RandPhraseSimple(RandSwear() + "За это надо выпить еще кружечку...",
                                 "Хорошо. Давай еще по одной.");
		link.l1.go = "sit_3";
		link.l2 = RandPhraseSimple("Спасибо, долж"+ GetSexPhrase("ен","на") +" откланяться.",
                                 "Эх, забористый ром. Ладно, мне уже пора, удачно посидеть.");
		link.l2.go = "exit_sit";
	break;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		case "rumours_trader":
            if (!CheckAttribute(NPChar, "quest.repeat.rumours_trader") || NPChar.quest.repeat.rumours_trader!= 3 )
            strum = SelectRumourEx("trader", NPChar);
            else strum = NO_RUMOUR_TEXT[rand(SIMPLE_RUMOUR_NUM - 1)]; // fix
			Dialog.Text =  NPCStringReactionRepeat(strum, strum+" Да. Больше ничего не припомню.",
                                                   strum+" А сейчас мне нужно вернуться к делам.",
                                                    RandPhraseSimple("Я ведь уже сказал, что у меня нет времени на пустую болтовню!",
                                                                        "Я занятой человек и мне некогда обсуждать всякие сплетни!"),
                                                                         "block", 1, npchar, Dialog.CurrentNode);
			link.l1 = HeroStringReactionRepeat(pcharrepphrase(RandSwear() + "Это очень интересно! Но давай к делу","Благодарю, тогда вот еще какое дело."),
                                                pcharrepphrase(""+ GetSexPhrase("Чтоб я сдох","Разрази меня гром") +"! Складно врешь. Но у меня есть еще вопрос.",
                                                                "Так не будем больше терять драгоценное время и перейдем к делам."),
                                                pcharrepphrase(RandSwear() +" Еще одно дельце, и я уйду.","Вы правы, время - деньги."),
                                                pcharrepphrase("Только и думаешь, что о своей выгоде. Ладно, не кипятись.",
                                                                "Прошу простить меня. Перейдем к делу."),
                                                npchar, Dialog.CurrentNode);

			link.l1.go = "business";
			link.l2 = HeroStringReactionRepeat(NPCharRepPhrase(npchar,
			                                        pcharrepphrase("Пока, старый пропойца.",
													               "Всего доброго, "+GetAddress_FormToNPC(NPChar)+" " +GetFullName(npchar)+"."),
													pcharrepphrase("Каррамба! Никуда не уходи. Я еще зайду.",
													               "Приятно было вас видеть, "+GetAddress_FormToNPC(NPChar)+" " +GetFullName(npchar)+"! До свидания!")),
											   NPCharRepPhrase(npchar,
											        pcharrepphrase("Отдать швартовы!",
													               "До свидания, милейший."),
													pcharrepphrase("Эй! Ты что там прячешь? Ром? Нет!?! Ну ладно, пойду горло промочу.",
													               "Прошу меня извинить, у меня срочные дела в городе.")),
											   NPCharRepPhrase(npchar,
											        pcharrepphrase("Хех! Еще увидимся!",
													               "Мне надо идти. Прощайте, "+GetAddress_FormToNPC(NPChar)+" " +GetFullName(npchar)+"!"),
													pcharrepphrase("И не смей мне перечить! Недосуг мне тут с тобой спорить!",
													               "Пожалуй, стоит проверить, как дела на корабле. Прощайте, "+GetAddress_FormToNPC(NPChar)+" " +GetFullName(npchar)+"!")),
											   NPCharRepPhrase(npchar,
											        pcharrepphrase("Что с тобой, старина? Это же я - " +GetFullName(pchar)+ "! Проспись, я потом зайду.",
													               "Боюсь, ваша печень прикончит вас раньше, чем я еще раз зайду сюда."),
													pcharrepphrase("Считай свои пиастры, пройдоха! Я еще зайду.",
													               "Рад"+ GetSexPhrase("","а") +" был"+ GetSexPhrase("","а") +" вас видеть, " + GetAddress_FormToNPC(NPChar)+" " +GetFullName(npchar)+"!")),
											  npchar, Dialog.CurrentNode);
			link.l2.go = "Exit";
		break;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        case "rumours_capitan":
			Dialog.Text = SelectRumourEx("capitan", NPChar);
			Link.l1 = RandPhraseSimple(RandSwear() + "Это очень интересно! Вот что еще хотел"+ GetSexPhrase("","а") +" спросить...",
                                     "Еще вопрос.");
			Link.l1.go = "quests";
			Link.l2 = RandPhraseSimple("Спасибо, долж"+ GetSexPhrase("ен","на") +" откланяться.",
                                     "Всего хорошего");
			Link.l2.go = "exit";
			NextDiag.TempNode = "quests";
            NPChar.talk_date =  lastspeakdate();
		break;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		case "rumours_tavern":  // homo 03/08/06

                 Dialog.Text = NPCStringReactionRepeat(LinkRandPhrase("Сведения стоят денег. За скромные 1000 золотых я расскажу все, что знаю.",
                "Что-то я запамятовал. Но, возможно, 1000 монет освежат мою память.",
                "Я готов рассказать все, что знаю. Но не бесплатно - 1000 золотых, на меньшее я не согласен."),
                "Больше мне нечего добавить. А теперь прошу меня извинить, но дела не ждут.",
                "", "", "block", 1, npchar, Dialog.CurrentNode);
             
    			link.l1 = HeroStringReactionRepeat(RandPhraseSimple(RandSwear() + "Платить за пустую болтовню? Нет уж, поищи кого-нибудь другого.",
                                         "За такие деньги я сам"+ GetSexPhrase("","а") +" все узнаю."), "Давай поговорим о чем-нибудь другом.", "Давай поговорим о чем-нибудь другом.", "Давай поговорим о чем-нибудь другом.", npchar, Dialog.CurrentNode);
                link.l1.go = HeroStringReactionRepeat("rumours_tavern_no", "first time", "first time", "first time", npchar, Dialog.CurrentNode);//"rumours_tavern_no";
                NextDiag.CurrentNode = NextDiag.TempNode;
    			if (makeint(pchar.money) >= 1000 )
    			{
    				link.l2 = HeroStringReactionRepeat(RandPhraseSimple("Ты их получишь, если я получу действительно ценные сведения.", "Я заплачу, но только при условии, что ты мне расскажешь действительно что-то важное."),
                     "", "", "" , npchar, Dialog.CurrentNode);
    				link.l2.go = HeroStringReactionRepeat("rumours_tavern_yes", "exit", "exit", "exit", npchar, Dialog.CurrentNode);
    			}
    			else
    			{
                    link.l2 =  HeroStringReactionRepeat("У меня нет такой суммы.", "", "", "", npchar, Dialog.CurrentNode);
    				link.l2.go = HeroStringReactionRepeat("rumours_tavern_no", "exit", "exit", "exit", npchar, Dialog.CurrentNode);
    			}

		break;
		
		case "rumours_tavern_yes":		
            string RumText = SelectRumourEx("tavern", NPChar); //fix
			
		    if(!RumourHasInformation(RumText))
            {
                Dialog.Text = NO_RUMOUR_TEXT[rand(SIMPLE_RUMOUR_NUM - 1)]; //fix
                link.l1 = RandPhraseSimple("Я не стану платить за пустые сплетни, прощай.", "Твои сведения не стоят и ломаного гроша - ты ничего не получишь.");

			    link.l1.go = "Exit";
			    NextDiag.CurrentNode = NextDiag.TempNode;
            }
            else
            {

                AddMoneyToCharacter(pchar, -1000);

                Dialog.Text = RumText;

    			link.l1 = RandPhraseSimple("Благодарю, вот твое золото. А больше ничего не припомнишь?", "Хорошо, ты заслужил свои деньги - держи. Есть что-нибудь еще?");

    			link.l1.go = "rumours_tavern";
    			NextDiag.CurrentNode = NextDiag.TempNode;
            }

		break;

		case "rumours_tavern_no":
			Dialog.Text = "Поймите меня правильно, мне нужно кормить семью.";
            link.l1 = "Давай поговорим о чем-нибудь другом.";
			link.l1.go = "first time";
			NextDiag.CurrentNode = NextDiag.TempNode;
			link.l2 = LinkRandPhrase("Спасибо, долж"+ GetSexPhrase("ен","на") +" откланяться.",
                                     "Всего хорошего","До свидания.");
			link.l2.go = "Exit";
		break;	
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        case "rumours_poor":
			if (!CheckAttribute(NPChar, "quest.repeat.rumours_poor") || NPChar.quest.repeat.rumours_poor <= 1)
			{
				Dialog.Text = SelectRumourEx("poor", NPChar);
				Link.l1 = RandPhraseSimple(RandSwear() + "Да, это интересно. Имеется еще вопрос...",
										"Хм, вот что еще хотел"+ GetSexPhrase("","а") +" спросить...");
				Link.l1.go = "rumours_poor_2";
				Link.l2 = RandPhraseSimple("Ну, прощай, барыга.",
										"Всего хорошего, приятель.");
				Link.l2.go = "exit";
				NPChar.quest.repeat.rumours_poor = sti(NPChar.quest.repeat.rumours_poor) + 1;
			}
			else 
			{
				Dialog.Text = RandPhraseSimple("Не о чем говорить, все уже выболтал, что знал.", 
					"Больше я ничего не слышал, извините...");
				Link.l1 = RandPhraseSimple("Понятно. Ну что же, всего хорошего.",
										"Ясно. Ну, бывай, приятель...");
				Link.l1.go = "exit";			
			}
            NPChar.talk_date = lastspeakdate();
		break;
        case "rumours_poor_2":
			if (NPChar.quest.repeat.rumours_poor <= 1)
			{
				Dialog.Text = SelectRumourEx("poor", NPChar);
				Link.l1 = RandPhraseSimple(RandSwear() + "Да, это интересно. Имеется еще вопрос...",
										"Хм, вот что еще хотел"+ GetSexPhrase("","а") +" спросить...");
				Link.l1.go = "rumours_poor";
				Link.l2 = RandPhraseSimple("Ну, прощай, барыга.",
										"Всего хорошего, приятель.");
				Link.l2.go = "exit";
				NPChar.quest.repeat.rumours_poor = sti(NPChar.quest.repeat.rumours_poor) + 1;
			}
			else 
			{
				Dialog.Text = RandPhraseSimple("Не о чем говорить, все уже выболтал, что знал.", 
					"Больше я ничего не слышал, извините...");
				Link.l1 = RandPhraseSimple("Понятно. Ну что же, всего хорошего.",
										"Ясно. Ну, бывай, приятель...");
				Link.l1.go = "exit";			
			}
			NPChar.talk_date = lastspeakdate();
		break;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        case "rumours_LSC":
			if (CheckNPCQuestDate(NPChar, "quest.repeat.rumours"))
			{
				Dialog.Text = SelectRumourExSpecial("LSC", NPChar);
				Link.l1 = RandPhraseSimple("Понятно...",
										"Ясно...");
				Link.l1.go = "exit";
				SetNPCQuestDate(NPChar, "quest.repeat.rumours");
			}
			else  
			{
				Dialog.Text = NPCharSexPhrase(npchar, RandPhraseSimple("Не о чем говорить, все уже выболтал, что знал.", "Больше я ничего не слышал, прошу прощения..."), 
					RandPhraseSimple("Простите, но я вам уже все выболтала.", "Больше я ничего не слышала, извините..."));
				Link.l1 = RandPhraseSimple("Я понял"+ GetSexPhrase("","а") +".", "Ну, всего хорошего.");
				Link.l1.go = "exit";			
			}
		break;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////		Кредитный генератор Loan
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		case "LoanForAll": //пересылка в финансы для всех
			dialog.text = LinkRandPhrase("Финансы? Хм, говорите, вас слушаю.", "Финансовые вопросы - это всегда интересно. Говорите, я слушаю вас.", "Что же, давайте поговорим о финансах.");
            if (CheckAttribute(pchar, "GenQuest.Loan.FindCitizen") && !CheckAttribute(pchar, "GenQuest.Loan.FindCitizenDone") && !CheckAttribute(pchar, "GenQuest.Loan.FindCitizenFalied"))
            {
                if (Characters[sti(pchar.GenQuest.Loan.FindCitizenIdx)].id == Npchar.id)
                {
                    dialog.text = LinkRandPhrase("А вы уверены, что обратились по адресу, " + GetAddress_Form(NPChar) + "? Мое имя " + GetFullName(npchar) + ". Чем могу служить?",
                             "По финансам ко мне, " + GetAddress_Form(NPChar) + "?! Я " + GetFullName(npchar) + ". А что за дело у вас?",
                             "Я вас слушаю, " + GetAddress_Form(NPChar) + ". Что вы хотели? Кстати, меня зовут " + GetFullName(npchar) + ".");
                    link.l1 = NPCharRepPhrase(pchar, "Я от ростовщика, должник! Деньги на бочку!", "У меня к вам щекотливый вопрос по вашему долгу, " + GetAddress_FormToNPC(NPChar) + ".");
                    link.l1.go = "LoanFindCitizen_1";
				    NPChar.quest.LoanFindCitizenType = rand(2); // три типа
                    break;
                }
            }
			if (CheckAttribute(pchar, "GenQuest.LoanChest.TakeChest") && sti(pchar.GenQuest.LoanChest.TargetIdx) == sti(NPChar.index))
            {
                link.l2 = "Я привез"+ GetSexPhrase("","ла") +" ваш кредит.";
                link.l2.go = "TakeChest_2";
            }
			link.l9 = RandPhraseSimple("М-м-м, я, видимо, ошиб"+ GetSexPhrase("ся","лась") +". Никаких финансовых дел я с вами не веду.", "Простите, никаких финансовых вопросов у меня к вам нет.");
			link.l9.go = "exit";
		break;
		// вернуть долг ростовщику
		case "LoanFindCitizen_1":
            switch (sti(pchar.GenQuest.Loan.FindCitizenChance1))
            {
                case 0 :
                    dialog.text = "Да, точно. Я как раз собирался ему все отдать.";
        			Link.l1 = "Ну что же, очень хорошо! Итого с тебя " + FindRussianMoneyString(makeint(sti(pchar.GenQuest.Loan.FindCitizenMoney) + sti(pchar.GenQuest.Loan.FindCitizenPercent))) + ".";
        			Link.l1.go = "LoanFindCitizenType_0";
        			Link.l2 = "Замечательно, вот и отнеси все сам!";
        			Link.l2.go = "exit";
                break;
                
                case 1 :
                    dialog.text = "Нет! Ничего я не отдам! А будете приставать, стражу позову.";
                    Link.l1 = "Ничего не знаю, с тебя " + FindRussianMoneyString(makeint(sti(pchar.GenQuest.Loan.FindCitizenMoney) + sti(pchar.GenQuest.Loan.FindCitizenPercent))) + ". А то можно поговорить и по-другому.";
        			Link.l1.go = "LoanFindCitizenType_1";
        			Link.l2 = "Ах ты, негодяй! Ну ничего, Земля круглая, свидимся еще...";
        			Link.l2.go = "LoanFindCitizenType_1_1";
                break;
                
                case 2 :
                    dialog.text = "К сожалению, у меня нет всей суммы.";
                    Link.l1 = "Ничего не знаю, с тебя " + FindRussianMoneyString(makeint(sti(pchar.GenQuest.Loan.FindCitizenMoney) + sti(pchar.GenQuest.Loan.FindCitizenPercent))) + ". А то можно поговорить и по-другому.";
        			Link.l1.go = "LoanFindCitizen_1_1";
        			Link.l2 = "А сколько есть?";
        			Link.l2.go = "LoanFindCitizenType_2";
        			//NPChar.quest.LoanFindCitizenType = rand(1);// первые 2 типа
                break;
			}
		break;
		
		case "LoanFindCitizen_1_1":
            switch (sti(pchar.GenQuest.Loan.FindCitizenChance2))
            {
                case 0 :
                    dialog.text = "Да, точно. Я как раз собирался ему все отдать.";
        			Link.l1 = "Ну что же, очень хорошо! Итого с тебя " + FindRussianMoneyString(makeint(sti(pchar.GenQuest.Loan.FindCitizenMoney) + sti(pchar.GenQuest.Loan.FindCitizenPercent))) + ".";
        			Link.l1.go = "LoanFindCitizenType_0";
        			Link.l2 = "Замечательно, вот и отнеси все сам!";
        			Link.l2.go = "exit";
                break;
                
                case 1 :
                    dialog.text = "Нет! Ничего я не отдам! А будете приставать, стражу позову.";
                    Link.l1 = "Ничего не знаю, с тебя " + FindRussianMoneyString(makeint(sti(pchar.GenQuest.Loan.FindCitizenMoney) + sti(pchar.GenQuest.Loan.FindCitizenPercent))) + ". А то можно поговорить и по-другому.";
        			Link.l1.go = "LoanFindCitizenType_1";
        			Link.l2 = "Ах ты, негодяй! Ну ничего, Земля круглая, свидимся еще...";
        			Link.l2.go = "LoanFindCitizenType_1_1";
                break;
			}
		break;
		
			case "LoanFindCitizen_1_2":
            switch (sti(pchar.GenQuest.Loan.FindCitizenChance3))
            {
                case 0 :
                    dialog.text = "Да, точно. Я как раз собирался ему все отдать.";
        			Link.l1 = "Ну что же, очень хорошо! Итого с тебя " + FindRussianMoneyString(makeint(sti(pchar.GenQuest.Loan.FindCitizenMoney) + sti(pchar.GenQuest.Loan.FindCitizenPercent))) + ".";
        			Link.l1.go = "LoanFindCitizenType_0";
        			Link.l2 = "Замечательно, вот и отнеси все сам!";
        			Link.l2.go = "exit";
                break;
                
                case 1 :
                    dialog.text = "Нет! Ничего я не отдам! А будете приставать, стражу позову.";
                    Link.l1 = "Ничего не знаю, с тебя " + FindRussianMoneyString(makeint(sti(pchar.GenQuest.Loan.FindCitizenMoney) + sti(pchar.GenQuest.Loan.FindCitizenPercent))) + ". А то можно поговорить и по-другому.";
        			Link.l1.go = "LoanFindCitizenType_1";
        			Link.l2 = "Ах ты, негодяй! Ну ничего, Земля круглая, свидимся еще...";
        			Link.l2.go = "LoanFindCitizenType_1_1";
                break;
			}
		break;

		case "LoanFindCitizenType_0":
            dialog.text = "Вот вся сумма.";
			Link.l1 = "Спасибо за сотрудничество!";
			Link.l1.go = "exit";
			pchar.GenQuest.Loan.FindCitizenDone = true;
        	addMoneyToCharacter(Pchar, (sti(pchar.GenQuest.Loan.FindCitizenMoney) + sti(pchar.GenQuest.Loan.FindCitizenPercent)));
        	SetTimerCondition("LoanFindCitizenTimer", 0, 0, 30, false);
        	Log_SetStringToLog("У вас есть 30 дней, чтобы доложить работодателю и вернуть ему деньги.");
        	AddCharacterExpToSkill(pchar, "COMMERCE", 100);  
		AddCharacterExpToSkill(pchar, "Fortune", 50);      	
        	AddQuestRecord("Gen_LoanFindCitizen", "3");
			npchar.LifeDay = 0;
		break;
		
		case "LoanFindCitizenType_1_1":
            dialog.text = "Рад, что мы с вами договорились.";
			Link.l1 = "Ты меня еще попомнишь!";
			Link.l1.go = "exit";
			pchar.GenQuest.Loan.FindCitizenFalied = true;
        	SetTimerCondition("LoanFindCitizenTimer", 0, 0, 30, false);
        	Log_SetStringToLog("У вас есть 30 дней, чтобы доложить работодателю о провале миссии.");
        	AddQuestRecord("Gen_LoanFindCitizen", "2");
			npchar.LifeDay = 0;
		break;
		
		case "LoanFindCitizenType_1":
            dialog.text = "Стража!! Хулиган"+ GetSexPhrase("ы","ки") +" зрения лишают!";
			Link.l1 = "Замолчи!";
			Link.l1.go = "fight";
			pchar.GenQuest.Loan.FindCitizenFalied = true;
        	SetTimerCondition("LoanFindCitizenTimer", 0, 0, 30, false);
        	Log_SetStringToLog("У вас есть 30 дней, чтобы доложить работодателю о провале миссии.");
        	AddQuestRecord("Gen_LoanFindCitizen", "2");
			npchar.LifeDay = 0;
			AddCharacterExpToSkill(pchar, "Sneak", 150);
		break;
		
		case "LoanFindCitizenType_2":
            pchar.GenQuest.Loan.FindCitizenTempMoney = makeint(sti(pchar.GenQuest.Loan.FindCitizenMoney) - rand(sti(pchar.GenQuest.Loan.FindCitizenPercent)));
            dialog.text = "Только " + pchar.GenQuest.Loan.FindCitizenTempMoney + " золотых.";
			Link.l1 = "Нет. Нужно всю сумму!";
			Link.l1.go = "LoanFindCitizen_1_2";
			Link.l2 = "Давай, что есть. Сам"+ GetSexPhrase("","а") +" доплачу. Так"+ GetSexPhrase("ой","ая") +" я сегодня добр"+ GetSexPhrase("ый","ая") +".";
			Link.l2.go = "LoanFindCitizenType_2_1";
			//NPChar.quest.LoanFindCitizenType = rand(1);// первые 2 типа
		break;
		
		case "LoanFindCitizenType_2_1":
            dialog.text = "Вы очень добры! Большое спасибо!";
			Link.l1 = "Эх! Вечно сам"+ GetSexPhrase("","а") +" во все лезу.";
			Link.l1.go = "exit";
			pchar.GenQuest.Loan.FindCitizenDone = true;
        	addMoneyToCharacter(Pchar, sti(pchar.GenQuest.Loan.FindCitizenTempMoney));
        	SetTimerCondition("LoanFindCitizenTimer", 0, 0, 30, false);
        	Log_SetStringToLog("У вас есть 30 дней, чтобы доложить работодателю и вернуть ему деньги.");
        	OfficersReaction("good");
        	ChangeCharacterReputation(pchar, 3);
        	AddCharacterExpToSkill(pchar, "Leadership", 30);
        	AddQuestRecord("Gen_LoanFindCitizen", "3");
			npchar.LifeDay = 0;
		break;
		//доставка сундуков
        case "TakeChest_2":
            if (GetCharacterItem(pchar, "Chest") >= sti(pchar.GenQuest.LoanChest.Chest))
            {
                if (CheckAttribute(pchar, "GenQuest.LoanChest.Money.Discount"))
				{
					Dialog.Text = "Все это хорошо, и я принимаю у вас золото. Однако, вы не уложились в отведенные вам сроки доставки. Поэтому я вынужден сократить размер вашего гонорара на " + pchar.GenQuest.LoanChest.Money.Discount + "%. Прошу прощения, но это обсуждению не подлежит. Извольте получить...";
					link.l1 = "Спасибо!";
    				link.l1.go = "exit";
					AddMoneyToCharacter(pchar, sti(pchar.GenQuest.LoanChest.Money)/100*sti(pchar.GenQuest.LoanChest.Money.Discount));
				}
				else
				{
					Dialog.Text = "Замечательно! Я давно его жду. Вот ваша награда.";
					link.l1 = "Спасибо!";
    				link.l1.go = "exit";
					AddMoneyToCharacter(pchar, sti(pchar.GenQuest.LoanChest.Money));
				}
				ChangeCharacterReputation(pchar, 1);
                AddCharacterExpToSkill(pchar, "Sailing", 50);
				AddCharacterExpToSkill(pchar, "Leadership", 20);
				AddCharacterExpToSkill(pchar, "COMMERCE", 5);
                OfficersReaction("good");
    			TakeNItems(pchar, "Chest", -sti(pchar.GenQuest.LoanChest.Chest));    			
    			DeleteAttribute(pchar, "GenQuest.LoanChest");
    			DeleteAttribute(Pchar, "quest.LoanChestTimer");
    			AddQuestRecord("Gen_LoanTakeChest", "3");
                CloseQuestHeader("Gen_LoanTakeChest");
			}
			else
			{
                Dialog.Text = "Замечательно! Но где же все сундуки с золотом?";
                link.l1 = "А вы можете принять золото без сундуков?";
    			link.l1.go = "TakeChest_3";
                link.l2 = "Зайду позже.";
    			link.l2.go = "exit";
			}
		break;

		case "TakeChest_3":
            Dialog.Text = "Нет! Только в сундуках!";
            link.l1 = "Плохо. Крысы у меня их попортили.";
			link.l1.go = "exit";
			//link.l2 = "Ну а, скажем, сумма в " + FindRussianMoneyString(sti(pchar.GenQuest.LoanChest.Chest)*30000) + " покроет стоимость сундуков?";
			//link.l2.go = "TakeChest_4";
		break;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////		Квесты мэра, проникновение во враждебный город
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		case "IntelligenceForAll":
            Dialog.Text = RandPhraseSimple("Ну что же, очень хорошо. У меня готов для вас пакет, который вам надлежит доставить известному лицу лично в руки. Извольте получить.\nНе подходите ко мне больше и не заговаривайте со мной, это может быть подозрительным. Соблюдайте осторожность...", 
				"Хм, прекрасно. Я вручаю вам пакет, и помните, что он должен быть доставлен названному вами лицу лично в руки.\nТеперь прощайте и не подходите ко мне более - это опасно.");
            link.l1 = RandPhraseSimple("Я понял"+ GetSexPhrase("","а") +". Прощайте...", "Прощайте и удачи.");
			link.l1.go = "exit";
			pchar.GenQuest.Intelligence = "Execute";
			GiveItem2Character(pchar, "sicretLetter");
			AddCharacterExpToSkill(pchar, "Sneak", 200);
			ref rItem = ItemsFromID("sicretLetter");
			rItem.City = XI_ConvertString("Colony"+characters[GetCharacterIndex(pchar.GenQuest.Intelligence.MayorId)].city+"Gen")); 
			rItem.Mayor = GetFullName(characterFromId(pchar.GenQuest.Intelligence.MayorId)); 
			AddQuestRecord("MayorsQuestsList", "7");
			AddQuestUserData("MayorsQuestsList", "ColonyName", XI_ConvertString("Colony"+characters[GetCharacterIndex(pchar.GenQuest.Intelligence.MayorId)].city+"Gen"));
			AddQuestUserData("MayorsQuestsList", "MayorName", GetFullName(characterFromId(pchar.GenQuest.Intelligence.MayorId)));
			AddQuestUserData("MayorsQuestsList", "sCity", XI_ConvertString("Colony"+pchar.GenQuest.Intelligence.City+"Dat"));
			AddQuestUserData("MayorsQuestsList", "sWho", GetWorkTypeOfMan(&characters[GetCharacterIndex(pchar.GenQuest.Intelligence.SpyId)], "Gen"));
			AddQuestUserData("MayorsQuestsList", "SpyName", GetFullName(&characters[GetCharacterIndex(pchar.GenQuest.Intelligence.SpyId)]));			
		break;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////		Грабеж среди бела дня, попытка залезть в сундуки
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        case "Man_FackYou":
			dialog.text = LinkRandPhrase("Грабеж среди бела дня!!! Это что же такое делается?! Ну, погоди, "+ GetSexPhrase("приятель","подруга") +"...", "Эй, ты чего это там копаешься?! Никак, вздумал"+ GetSexPhrase("","а") +" ограбить меня? Ну, тогда тебе конец...", "Постой, ты куда это полез"+ GetSexPhrase("","ла") +"? Да ты вор"+ GetSexPhrase("","овка") +", оказывается! Ну, считай, что ты приплыл"+ GetSexPhrase("","а") +", родн"+ GetSexPhrase("ой","ая") +"...");
			link.l1 = LinkRandPhrase("Дьявол!!", "Каррамба!!", "А-ать, черт!");
			link.l1.go = "fight_owner";
		break;
		case "fight_owner":
			LAi_SetOwnerTypeNoGroup(NPChar);
			LAi_group_Attack(NPChar, Pchar);
			if (rand(3) != 1) SetNationRelation2MainCharacter(sti(npchar.nation), RELATION_ENEMY);
			NextDiag.CurrentNode = "First time";
			DialogExit();
		break;
	}
}

string sRumourAboutOwners_CityRumour(string sCity, string sOwnerType) // Здесь идет распределение по городам
{
	string sRetRumour = "Явный баг. Сообщите о нем АТ, для Warship'а.";
	// флаг - PChar.QuestTemp.RumoursAboutOwners.(sOwner) - для проверок, какой вопрос выпал. Ножно будет юзать для квестов
	switch(sCity)
	{
		case "Tortuga":
			sRetRumour = sRumourAboutOwners_Init(sOwnerType, 0);
			PChar.QuestTemp.RumoursAboutOwners.(sOwnerType) = 0;
		break;
		
		case "PortRoyal":
			sRetRumour = sRumourAboutOwners_Init(sOwnerType, 1);
			PChar.QuestTemp.RumoursAboutOwners.(sOwnerType) = 1;
		break;
		
		case "FortOrange":
			sRetRumour = sRumourAboutOwners_Init(sOwnerType, 2);
			PChar.QuestTemp.RumoursAboutOwners.(sOwnerType) = 2;
		break;
		
		case "Beliz":
			sRetRumour = sRumourAboutOwners_Init(sOwnerType, 3);
			PChar.QuestTemp.RumoursAboutOwners.(sOwnerType) = 3;
		break;
		
		case "PortoBello":
			sRetRumour = sRumourAboutOwners_Init(sOwnerType, 4);
			PChar.QuestTemp.RumoursAboutOwners.(sOwnerType) = 4;
		break;
		
		case "Cartahena":
			sRetRumour = sRumourAboutOwners_Init(sOwnerType, 5);
			PChar.QuestTemp.RumoursAboutOwners.(sOwnerType) = 5;
		break;
		
		case "Maracaibo":
			sRetRumour = sRumourAboutOwners_Init(sOwnerType, 6);
			PChar.QuestTemp.RumoursAboutOwners.(sOwnerType) = 6;
		break;
		
		case "Caracas":
			sRetRumour = sRumourAboutOwners_Init(sOwnerType, 0);
			PChar.QuestTemp.RumoursAboutOwners.(sOwnerType) = 7;
		break;
		
		case "Cumana":
			sRetRumour = sRumourAboutOwners_Init(sOwnerType, 1);
			PChar.QuestTemp.RumoursAboutOwners.(sOwnerType) = 8;
		break;
		
		case "SantaCatalina":
			sRetRumour = sRumourAboutOwners_Init(sOwnerType, 2);
			PChar.QuestTemp.RumoursAboutOwners.(sOwnerType) = 9;
		break;
		
		case "SanJuan":
			sRetRumour = sRumourAboutOwners_Init(sOwnerType, 3);
			PChar.QuestTemp.RumoursAboutOwners.(sOwnerType) = 10;
		break;
		
		case "Marigo":
			sRetRumour = sRumourAboutOwners_Init(sOwnerType, 4);
			PChar.QuestTemp.RumoursAboutOwners.(sOwnerType) = 11;
		break;
		
		case "Charles":
			sRetRumour = sRumourAboutOwners_Init(sOwnerType, 5);
			PChar.QuestTemp.RumoursAboutOwners.(sOwnerType) = 12;
		break;
		
		case "SentJons":
			sRetRumour = sRumourAboutOwners_Init(sOwnerType, 6);
			PChar.QuestTemp.RumoursAboutOwners.(sOwnerType) = 13;
		break;
		
		case "FortFrance":
			sRetRumour = sRumourAboutOwners_Init(sOwnerType, 0);
			PChar.QuestTemp.RumoursAboutOwners.(sOwnerType) = 14;
		break;
		
		case "BasTer":
			sRetRumour = sRumourAboutOwners_Init(sOwnerType, 1);
			PChar.QuestTemp.RumoursAboutOwners.(sOwnerType) = 15;
		break;
		
		case "Bridgetown":
			sRetRumour = sRumourAboutOwners_Init(sOwnerType, 2);
			PChar.QuestTemp.RumoursAboutOwners.(sOwnerType) = 16;
		break;
		
		case "PortSpein":
			sRetRumour = sRumourAboutOwners_Init(sOwnerType, 3);
			PChar.QuestTemp.RumoursAboutOwners.(sOwnerType) = 17;
		break;
		
		case "Havana":
			sRetRumour = sRumourAboutOwners_Init(sOwnerType, 4);
			PChar.QuestTemp.RumoursAboutOwners.(sOwnerType) = 18;
		break;
		
		case "Villemstad":
			sRetRumour = sRumourAboutOwners_Init(sOwnerType, 5);
			PChar.QuestTemp.RumoursAboutOwners.(sOwnerType) = 19;
		break;
		
		case "Santiago":
			sRetRumour = sRumourAboutOwners_Init(sOwnerType, 6);
			PChar.QuestTemp.RumoursAboutOwners.(sOwnerType) = 20;
		break;
		
		case "PortPax":
			sRetRumour = sRumourAboutOwners_Init(sOwnerType, 0);
			PChar.QuestTemp.RumoursAboutOwners.(sOwnerType) = 21;
		break;
		
		case "SantoDomingo":
			sRetRumour = sRumourAboutOwners_Init(sOwnerType, 1);
			PChar.QuestTemp.RumoursAboutOwners.(sOwnerType) = 22;
		break;
	}
	
	return sRetRumour;
}

string sRumourAboutOwners_Init(string sOwnerType, int iRumourNum) // База слухов жителей. Можно менять без новой игры
{
	
	string STR_MAYOR[MAX_RUMOURS_ABOUT_OWNERS]; // Губернаторы
	STR_MAYOR[0] = "О, господин губернатор - замечательный человек. Он умело управляет нашей колонией - с момента ее основания у нас не было ни единого крупного конфликта\nЕдинственный его недостаток - он... ммм... весьма бережлив. Хотя, возможно, это наоборот достоинство.";
	STR_MAYOR[1] = "О, господин губернатор - замечательный человек. Он умело управляет нашей колонией - с момента ее основания у нас не было ни единого крупного конфликта\nЕдинственный его недостаток - он... ммм... весьма бережлив. Хотя, возможно, это наоборот достоинство.";
	STR_MAYOR[2] = "Увы, господин губернатор у нас хороший... Но слабохарактерный. Ему бы чуточку твердости в душе - и у наших торговцев появились бы небольшие налоговые льготы... И субсидии на ремонт форта давно пора бы выделить.";
	STR_MAYOR[3] = "Хм... Он недавно стал у нас губернатором. Он довольно молод и поэтому очень инициативен - при нем наша колония процветает.";
	STR_MAYOR[4] = "Наш губернатор еще относительно молод. Говорят, он - непутевый младший сын испанского графа, отправленный от двора куда подальше, чтобы прекратил позорить свой род. Назначен он был сюда совсем недавно, и ничего запоминающегося сотворить не успел.";
	STR_MAYOR[5] = "Хм... Он недавно стал у нас губернатором. Он довольно молод и поэтому очень инициативен - при нем наша колония процветает.";
	STR_MAYOR[6] = "Э-э... Ничего... ничего плохого сказать про него не могу.";
	
	string STR_TAVERNKEEPER[MAX_RUMOURS_ABOUT_OWNERS]; // Тавернщики
	STR_TAVERNKEEPER[0] = "Хм, я бы не доверил свое имущество этому пройдохе. Не поймите меня неправильно, но мне кажется, что это самый ненадежный человек в нашем городке\nГоворят, что он путается с контрабандистами и пиратами - и в таверне у него все время всякий сброд ошивается!";
	STR_TAVERNKEEPER[1] = "Да что о нем можно сказать! Трусоват, да и умом не отличается. К тому же жаден до денег, отчего постоянно попадает в различные неприятности! Зато, правда, он всегда в курсе всех новостей.";
	STR_TAVERNKEEPER[2] = "Довольно приятный тип. У него в таверне всегда приятно посидеть, да и чувством юмора у хозяина все в порядке. А самое главное - он следит за порядком в таверне и не позволяет всякой пьяной швали задирать добропорядочных граждан.";
	STR_TAVERNKEEPER[3] = "Удивительный человек. Ему надо не таверну содержать, а осведомителем быть при тайной канцелярии. Иногда кажется, что ему известно буквально все, что происходит в нашем городе. Даже жуть пробирает.";
	STR_TAVERNKEEPER[4] = "Не знаю, чего уж вам о нем сказать. Человек как человек. Содержит таверну, соответственно, в курсе всех новостей нашего острова.";
	STR_TAVERNKEEPER[5] = "Говорят, на него можно положиться. С другой стороны, я слышал, что он не любит ненужного риска и всегда расчетливо ведет свои дела. Да и темными делишками вроде не занимается.";
	STR_TAVERNKEEPER[6] = "Таверна досталась ему от отца. Его отец содержал таверну не так далеко отсюда, на небольшом острове Хайрок в другом архипелаге. Потом отец перебрался сюда, построил здесь новую таверну, и теперь в ней заправляет его сын.";
	
	string STR_SHIPYARDER[MAX_RUMOURS_ABOUT_OWNERS]; // Верфисты
	STR_SHIPYARDER[0] = "Корабли он строит неплохие... А как о человеке, о нем трудно что-либо сказать - он живет тихо и спокойно. Наверное, неплохой человек.";
	STR_SHIPYARDER[1] = "Корабли он строит неплохие... А как о человеке, о нем трудно что-либо сказать - он живет тихо и спокойно. Наверное, неплохой человек.";
	STR_SHIPYARDER[2] = "Неплохой человек, но говорят, что он излишне суров. Постоянно отчитывает всех работников. Поэтому они часто у него меняются. Правда, не смотря на это, корабел он превосходный.";
	STR_SHIPYARDER[3] = "Он очень образованный человек. Говорят, что он учился кораблестроению в Англии, потом в Голландии. А в конце концов оказался здесь - вдали от метрополии, в захолустном архипелаге\nГоворят, что он не поладил в свое время со Святой Инквизицией, и ему пришлось бежать в колонии.";
	STR_SHIPYARDER[4] = "Да, этот старик уже никуда не годится. Он постоянно грезит о прошедших временах, и ворчит, когда ему заказывают что-нибудь, кроме каравеллы или галеона.";
	STR_SHIPYARDER[5] = "Отличный парень. Всегда готов помочь, выручить из беды. Надо сказать, что и корабли для подобного захолустья он строит очень даже неплохие.";
	STR_SHIPYARDER[6] = "Он был ссыльным в нашей колонии, пока не оказалось, что у него талант к кораблестроению. После того, как под его руководством на верфях нашей колонии были построены два великолепных брига, ему простили его прегрешения, и теперь он - полноправный член нашего общества.";
	
	string STR_TRADER[MAX_RUMOURS_ABOUT_OWNERS]; // Магазинщики
	STR_TRADER[0] = "Ничего особенно плохого о нем сказать не могу. Добропорядочный житель, усердный прихожанин нашей церкви. Никогда не слышал, чтобы кто-нибудь хулил его или был на него обижен.";
	STR_TRADER[1] = "Он главный поставщик нашего губернатора, и это дает ему неплохой доход. Но дела он свои ведет честно. Ну, или пока ни в чем позорящем его замечен не был.";
	STR_TRADER[2] = "Этот толстяк пользуется заслуженным уважением в нашем городе. Товар у него всегда отменный, и я не припомню случая, чтобы он кого-нибудь обсчитал.";
	STR_TRADER[3] = "Непутевый он малый. Торгует-торгует, а толку никакого. То одних товаров нет, то других. И сам из долгов никак не вылезет.";
	STR_TRADER[4] = "Нехороший он человек, мсье. Говорят, он был помощником Болдуина Коффье, но разорил его и потом перекупил магазин. Не знаю, как насчет качества товаров, которые он продает, но как человек он мне весьма неприятен.";
	STR_TRADER[5] = "О! Это мерзкий человек, я бы не советовал вам иметь с ним никаких дел\nОн держит половину жителей нашего города в должниках! А товары, которые продаются у него в магазине всегда низкого качества.";
	STR_TRADER[6] = "Да никто о нем ничего толком и не знает. Он недавно сюда переселился, и сразу открыл свое дело. Торгует вроде честно, контрабанды не возит.";
	
	string sTempMayor = STR_MAYOR[iRumourNum];
	string sTempTavernkeper = STR_TAVERNKEEPER[iRumourNum];
	string sTempShipyarder = STR_SHIPYARDER[iRumourNum];
	string sTempTrader = STR_TRADER[iRumourNum];
	
	if(sOwnerType == "Mayor") return sTempMayor;
	if(sOwnerType == "TavernKeeper") return sTempTavernkeper;
	if(sOwnerType == "ShipYarder") return sTempShipyarder;
	if(sOwnerType == "Trader") return sTempTrader;
}