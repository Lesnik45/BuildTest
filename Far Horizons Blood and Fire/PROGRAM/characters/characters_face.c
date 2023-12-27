string	sEnManFaces = "30,35,10,6,32,33,42,43";
string	sFrManFaces = "19,27,29,31,32,33,43,43";
string	sSpManFaces = "25,37,38,44,7,32,33,42,43";
string	sHoManFaces = "11,26,28,36,32,33,42,43";
string	sPoManFaces = "12,30,39,32,33,42,43";
string	sPiManFaces = "0,1,13,14,2,23,24,3,40,5";

void SetRandomFaceToCharacter(ref rCharacter)
{
	string sFaceID = "1";
	if (rCharacter.sex == "man")
	{
		switch (sti(rCharacter.nation))
		{
			case ENGLAND:	
				sFaceID = GetRandSubString(sEnManFaces); 
			break;
			case FRANCE:	
				sFaceID = GetRandSubString(sFrManFaces); 
			break;
			case SPAIN:		
				sFaceID = GetRandSubString(sSpManFaces); 
				break;
			case PIRATE:	
				sFaceID = GetRandSubString(sPiManFaces); 
				break;
			case HOLLAND:	
				sFaceID = GetRandSubString(sHoManFaces); 
				break;
		}
	}
	else
	{
		sFaceID = "16";
	}
	
	rCharacter.FaceId = sti(sFaceID);
}

void FaceMaker(aref rCharacter)
{
	switch (rCharacter.model)
	{
		// заглушка
		case "beatrice":		rCharacter.FaceId = 0; break;
		
		// пираты
		case "pirate_1":        rCharacter.FaceId = 4; break;
		case "pirate_2":        rCharacter.FaceId = 18; break;
		case "pirate_3":        rCharacter.FaceId = 5; break;
		case "pirate_4":        rCharacter.FaceId = 6; break;
		case "pirate_5":        rCharacter.FaceId = 7; break;
		case "pirate_6":        rCharacter.FaceId = 8; break;
		case "pirate_7":        rCharacter.FaceId = 9; break;
		case "pirate_8":        rCharacter.FaceId = 10; break;
		case "pirate_9":        rCharacter.FaceId = 11; break;
		case "pirate_10":       rCharacter.FaceId = 12; break;
		case "pirate_11":       rCharacter.FaceId = 13; break;
		case "pirate_12":       rCharacter.FaceId = 14; break;
		case "pirate_13":       rCharacter.FaceId = 3; break;
		case "pirate_14":       rCharacter.FaceId = 15; break;
		case "pirate_15":       rCharacter.FaceId = 16; break;
		case "pirate_16":       rCharacter.FaceId = 17; break;
		
		// пираты (мушкетёры)
		case "mush_pir_1":        rCharacter.FaceId = 14; break;
		case "mush_pir_2":        rCharacter.FaceId = 3; break;
		case "mush_pir_3":        rCharacter.FaceId = 4; break;
		case "mush_pir_4":        rCharacter.FaceId = 16; break;
		case "mush_pir_5":        rCharacter.FaceId = 17; break;
		case "mush_pir_6":        rCharacter.FaceId = 7; break;
	}
	
	if(!CheckAttribute(rCharacter, "FaceId")) rCharacter.FaceId = 0; //Экку Korsar - Если айди портрета нету, то ставим плейсхолдер
}

// boal 22.04.04 выбор модели зависимо от типа
void SetCaptanModelByEncType(ref Chref, string sFantomType)
{
    string ModelPirate = "Albermal"; // значит баг

	if (sti(Chref.nation) == PIRATE) sFantomType = "pirate"; // иначе баг
	
    switch (sFantomType)
	{
		case "trade":
            ModelPirate = "trader_1";
		break;
		case "war":
			ModelPirate = "off_" + NationShortName(sti(Chref.nation)) + "_1";
		break;
		case "pirate":
            ModelPirate = "officer_1";
		break;
	}
	Chref.model = ModelPirate;
	FaceMaker(Chref);
}

void SetModelPirate(ref ChrefPirate)
{
    string ModelPirate;
    
    ModelPirate = "pirate_" + (rand(15) + 1);
    
    ChrefPirate.model = ModelPirate;
    FaceMaker(ChrefPirate);
}

// Warship. Обновим персу модель и анимацию. Пока-что здесь
void Characters_RefreshModel(ref rChar)
{
	SendMessage(rChar, "lss", MSG_CHARACTER_SETMODEL, rChar.model, rChar.model.animation);
}