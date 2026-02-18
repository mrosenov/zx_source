#include "skillwrapper.h"
#include "skillfilter.h"
#include "skill.h"
#include "playerwrapper.h"


DEFINE_SUBSTANCE(filter_Disperseanti, filter, CLASS_DISPERSEANTI);
DEFINE_SUBSTANCE(filter_Frenzied, filter, CLASS_FRENZIED);
DEFINE_SUBSTANCE(filter_Qilinform, filter, CLASS_QILINFORM);
DEFINE_SUBSTANCE(filter_Insanityform, filter, CLASS_INSANITYFORM);
DEFINE_SUBSTANCE(filter_Dizzytimer, filter, CLASS_DIZZYTIMER);
//5
DEFINE_SUBSTANCE(filter_Drunk, filter, CLASS_DRUNK);
DEFINE_SUBSTANCE(filter_Damagemove, filter, CLASS_DAMAGEMOVE);
DEFINE_SUBSTANCE(filter_Tongxin, filter, CLASS_TONGXIN);
DEFINE_SUBSTANCE(filter_Subantiwrap2, filter, CLASS_SUBANTIWRAP2);
DEFINE_SUBSTANCE(filter_Sharestatewithclone, filter, CLASS_SHARESTATEWITHCLONE);
//10
DEFINE_SUBSTANCE(filter_Decskillaccu, filter, CLASS_DECSKILLACCU);
DEFINE_SUBSTANCE(filter_Yuanling, filter, CLASS_YUANLING);
DEFINE_SUBSTANCE(filter_Iceshield, filter, CLASS_ICESHIELD);
DEFINE_SUBSTANCE(filter_Cycsubattack, filter, CLASS_CYCSUBATTACK);
DEFINE_SUBSTANCE(filter_BarAdddogeaccu, filter, CLASS_BARADDDOGEACCU);
//15
DEFINE_SUBSTANCE(filter_Dimming, filter, CLASS_DIMMING);
DEFINE_SUBSTANCE(filter_Inchurt, filter, CLASS_INCHURT);
DEFINE_SUBSTANCE(filter_Cycsubdefence, filter, CLASS_CYCSUBDEFENCE);
DEFINE_SUBSTANCE(filter_Darkuniform, filter, CLASS_DARKUNIFORM);
DEFINE_SUBSTANCE(filter_Addantisilent, filter, CLASS_ADDANTISILENT);
//20
DEFINE_SUBSTANCE(filter_Addattack4, filter, CLASS_ADDATTACK4);
DEFINE_SUBSTANCE(filter_Hpleak2, filter, CLASS_HPLEAK2);
DEFINE_SUBSTANCE(filter_Setskilllevel, filter, CLASS_SETSKILLLEVEL);
DEFINE_SUBSTANCE(filter_Hpregain, filter, CLASS_HPREGAIN);
DEFINE_SUBSTANCE(filter_Darkness, filter, CLASS_DARKNESS);
//25
DEFINE_SUBSTANCE(filter_Triggerskill, filter, CLASS_TRIGGERSKILL);
DEFINE_SUBSTANCE(filter_Expboost, filter, CLASS_EXPBOOST);
DEFINE_SUBSTANCE(filter_Ningjin, filter, CLASS_NINGJIN);
DEFINE_SUBSTANCE(filter_Barexpboost, filter, CLASS_BAREXPBOOST);
DEFINE_SUBSTANCE(filter_Barmpgen, filter, CLASS_BARMPGEN);
//30
DEFINE_SUBSTANCE(filter_Shadowhide, filter, CLASS_SHADOWHIDE);
DEFINE_SUBSTANCE(filter_Shadowdance, filter, CLASS_SHADOWDANCE);
DEFINE_SUBSTANCE(filter_Powerless, filter, CLASS_POWERLESS);
DEFINE_SUBSTANCE(filter_Subhp, filter, CLASS_SUBHP);
DEFINE_SUBSTANCE(filter_Teamenchant, filter, CLASS_TEAMENCHANT);
//35
DEFINE_SUBSTANCE(filter_Immunesleep, filter, CLASS_IMMUNESLEEP);
DEFINE_SUBSTANCE(filter_Decaccuracy, filter, CLASS_DECACCURACY);
DEFINE_SUBSTANCE(filter_Skillreflect, filter, CLASS_SKILLREFLECT);
DEFINE_SUBSTANCE(filter_Wraptimer, filter, CLASS_WRAPTIMER);
DEFINE_SUBSTANCE(filter_Returnpos, filter, CLASS_RETURNPOS);
//40
DEFINE_SUBSTANCE(filter_Paralysis, filter, CLASS_PARALYSIS);
DEFINE_SUBSTANCE(filter_Autoresurrect, filter, CLASS_AUTORESURRECT);
DEFINE_SUBSTANCE(filter_Inchpgen, filter, CLASS_INCHPGEN);
DEFINE_SUBSTANCE(filter_Vipguilin, filter, CLASS_VIPGUILIN);
DEFINE_SUBSTANCE(filter_Incskilldamage, filter, CLASS_INCSKILLDAMAGE);
//45
DEFINE_SUBSTANCE(filter_Mirrorimage, filter, CLASS_MIRRORIMAGE);
DEFINE_SUBSTANCE(filter_Addcharm, filter, CLASS_ADDCHARM);
DEFINE_SUBSTANCE(filter_Deccrithurt, filter, CLASS_DECCRITHURT);
DEFINE_SUBSTANCE(filter_Decdamage, filter, CLASS_DECDAMAGE);
DEFINE_SUBSTANCE(filter_Familyincattack, filter, CLASS_FAMILYINCATTACK);
//50
DEFINE_SUBSTANCE(filter_Incfatalratio, filter, CLASS_INCFATALRATIO);
DEFINE_SUBSTANCE(filter_Activeonfilteradd, filter, CLASS_ACTIVEONFILTERADD);
DEFINE_SUBSTANCE(filter_Crazy, filter, CLASS_CRAZY);
DEFINE_SUBSTANCE(filter_Adddefence, filter, CLASS_ADDDEFENCE);
DEFINE_SUBSTANCE(filter_Hupo, filter, CLASS_HUPO);
//55
DEFINE_SUBSTANCE(filter_Immuneweak, filter, CLASS_IMMUNEWEAK);
DEFINE_SUBSTANCE(filter_Guishen2debuff, filter, CLASS_GUISHEN2DEBUFF);
DEFINE_SUBSTANCE(filter_Hpleak1, filter, CLASS_HPLEAK1);
DEFINE_SUBSTANCE(filter_Subantiweak, filter, CLASS_SUBANTIWEAK);
DEFINE_SUBSTANCE(filter_Dechurt2, filter, CLASS_DECHURT2);
//60
DEFINE_SUBSTANCE(filter_Incskillaccu, filter, CLASS_INCSKILLACCU);
DEFINE_SUBSTANCE(filter_Summonskill, filter, CLASS_SUMMONSKILL);
DEFINE_SUBSTANCE(filter_Immunesilent, filter, CLASS_IMMUNESILENT);
DEFINE_SUBSTANCE(filter_Juniordarkform, filter, CLASS_JUNIORDARKFORM);
DEFINE_SUBSTANCE(filter_Crazycurse, filter, CLASS_CRAZYCURSE);
//65
DEFINE_SUBSTANCE(filter_Puppetform, filter, CLASS_PUPPETFORM);
DEFINE_SUBSTANCE(filter_Sharelifewithsummon, filter, CLASS_SHARELIFEWITHSUMMON);
DEFINE_SUBSTANCE(filter_Gtmpgen, filter, CLASS_GTMPGEN);
DEFINE_SUBSTANCE(filter_Silenttimer, filter, CLASS_SILENTTIMER);
DEFINE_SUBSTANCE(filter_Subattack, filter, CLASS_SUBATTACK);
//70
DEFINE_SUBSTANCE(filter_Crippleddebuff, filter, CLASS_CRIPPLEDDEBUFF);
DEFINE_SUBSTANCE(filter_Familyincmp, filter, CLASS_FAMILYINCMP);
DEFINE_SUBSTANCE(filter_Subdefence, filter, CLASS_SUBDEFENCE);
DEFINE_SUBSTANCE(filter_Makecrit, filter, CLASS_MAKECRIT);
DEFINE_SUBSTANCE(filter_Addattack3, filter, CLASS_ADDATTACK3);
//75
DEFINE_SUBSTANCE(filter_Cursed, filter, CLASS_CURSED);
DEFINE_SUBSTANCE(filter_Rejectdebuff, filter, CLASS_REJECTDEBUFF);
DEFINE_SUBSTANCE(filter_Huju, filter, CLASS_HUJU);
DEFINE_SUBSTANCE(filter_Subantidizzy, filter, CLASS_SUBANTIDIZZY);
DEFINE_SUBSTANCE(filter_Adddodge2, filter, CLASS_ADDDODGE2);
//80
DEFINE_SUBSTANCE(filter_Subantisleep, filter, CLASS_SUBANTISLEEP);
DEFINE_SUBSTANCE(filter_Activebecrit, filter, CLASS_ACTIVEBECRIT);
DEFINE_SUBSTANCE(filter_Subantislow, filter, CLASS_SUBANTISLOW);
DEFINE_SUBSTANCE(filter_Sharelifewithclone, filter, CLASS_SHARELIFEWITHCLONE);
DEFINE_SUBSTANCE(filter_Xinmo, filter, CLASS_XINMO);
//85
DEFINE_SUBSTANCE(filter_Addcommon, filter, CLASS_ADDCOMMON);
DEFINE_SUBSTANCE(filter_Delaycast, filter, CLASS_DELAYCAST);
DEFINE_SUBSTANCE(filter_Addspirit, filter, CLASS_ADDSPIRIT);
DEFINE_SUBSTANCE(filter_Longxiang, filter, CLASS_LONGXIANG);
DEFINE_SUBSTANCE(filter_Subhp2, filter, CLASS_SUBHP2);
//90
DEFINE_SUBSTANCE(filter_Inchp2, filter, CLASS_INCHP2);
DEFINE_SUBSTANCE(filter_Instantskill, filter, CLASS_INSTANTSKILL);
DEFINE_SUBSTANCE(filter_Hpleak6, filter, CLASS_HPLEAK6);
DEFINE_SUBSTANCE(filter_Guishen1, filter, CLASS_GUISHEN1);
DEFINE_SUBSTANCE(filter_Blessed, filter, CLASS_BLESSED);
//95
DEFINE_SUBSTANCE(filter_Stateexchangewithclone, filter, CLASS_STATEEXCHANGEWITHCLONE);
DEFINE_SUBSTANCE(filter_Chihun, filter, CLASS_CHIHUN);
DEFINE_SUBSTANCE(filter_Pulling, filter, CLASS_PULLING);
DEFINE_SUBSTANCE(filter_Cycle, filter, CLASS_CYCLE);
DEFINE_SUBSTANCE(filter_Barhpgen, filter, CLASS_BARHPGEN);
//100
DEFINE_SUBSTANCE(filter_Incskilllevel, filter, CLASS_INCSKILLLEVEL);
DEFINE_SUBSTANCE(filter_Addwrap, filter, CLASS_ADDWRAP);
DEFINE_SUBSTANCE(filter_Addattack2, filter, CLASS_ADDATTACK2);
DEFINE_SUBSTANCE(filter_Deccritrate, filter, CLASS_DECCRITRATE);
DEFINE_SUBSTANCE(filter_Inchp, filter, CLASS_INCHP);
//105
DEFINE_SUBSTANCE(filter_Decskilldodge, filter, CLASS_DECSKILLDODGE);
DEFINE_SUBSTANCE(filter_Incattack3, filter, CLASS_INCATTACK3);
DEFINE_SUBSTANCE(filter_Activecrit, filter, CLASS_ACTIVECRIT);
DEFINE_SUBSTANCE(filter_Fly, filter, CLASS_FLY);
DEFINE_SUBSTANCE(filter_Pull, filter, CLASS_PULL);
//110
DEFINE_SUBSTANCE(filter_Addhp, filter, CLASS_ADDHP);
DEFINE_SUBSTANCE(filter_Addantiweak, filter, CLASS_ADDANTIWEAK);
DEFINE_SUBSTANCE(filter_Incanti, filter, CLASS_INCANTI);
DEFINE_SUBSTANCE(filter_Familyinchp, filter, CLASS_FAMILYINCHP);
DEFINE_SUBSTANCE(filter_Accumdamage, filter, CLASS_ACCUMDAMAGE);
//115
DEFINE_SUBSTANCE(filter_Spicy, filter, CLASS_SPICY);
DEFINE_SUBSTANCE(filter_Asfire, filter, CLASS_ASFIRE);
DEFINE_SUBSTANCE(filter_Addslow, filter, CLASS_ADDSLOW);
DEFINE_SUBSTANCE(filter_Dechurt, filter, CLASS_DECHURT);
DEFINE_SUBSTANCE(filter_Douhun, filter, CLASS_DOUHUN);
//120
DEFINE_SUBSTANCE(filter_Addantidizzy, filter, CLASS_ADDANTIDIZZY);
DEFINE_SUBSTANCE(filter_Aoshi, filter, CLASS_AOSHI);
DEFINE_SUBSTANCE(filter_Adddodge, filter, CLASS_ADDDODGE);
DEFINE_SUBSTANCE(filter_Focusanti, filter, CLASS_FOCUSANTI);
DEFINE_SUBSTANCE(filter_Addanti2, filter, CLASS_ADDANTI2);
//125
DEFINE_SUBSTANCE(filter_Guishen2, filter, CLASS_GUISHEN2);
DEFINE_SUBSTANCE(filter_Speedto, filter, CLASS_SPEEDTO);
DEFINE_SUBSTANCE(filter_Yanyu, filter, CLASS_YANYU);
DEFINE_SUBSTANCE(filter_Banruo, filter, CLASS_BANRUO);
DEFINE_SUBSTANCE(filter_Cycdecdefence, filter, CLASS_CYCDECDEFENCE);
//130
DEFINE_SUBSTANCE(filter_Cycleskill, filter, CLASS_CYCLESKILL);
DEFINE_SUBSTANCE(filter_Humanbomb, filter, CLASS_HUMANBOMB);
DEFINE_SUBSTANCE(filter_Aloof, filter, CLASS_ALOOF);
DEFINE_SUBSTANCE(filter_Juniorlightform, filter, CLASS_JUNIORLIGHTFORM);
DEFINE_SUBSTANCE(filter_Cibei, filter, CLASS_CIBEI);
//135
DEFINE_SUBSTANCE(filter_Decdrugeffect, filter, CLASS_DECDRUGEFFECT);
DEFINE_SUBSTANCE(filter_Incmpgen, filter, CLASS_INCMPGEN);
DEFINE_SUBSTANCE(filter_Decdefence, filter, CLASS_DECDEFENCE);
DEFINE_SUBSTANCE(filter_Goback, filter, CLASS_GOBACK);
DEFINE_SUBSTANCE(filter_Bleeding, filter, CLASS_BLEEDING);
//140
DEFINE_SUBSTANCE(filter_Divinityfury, filter, CLASS_DIVINITYFURY);
DEFINE_SUBSTANCE(filter_Equipeffect, filter, CLASS_EQUIPEFFECT);
DEFINE_SUBSTANCE(filter_Frozen, filter, CLASS_FROZEN);
DEFINE_SUBSTANCE(filter_Hpleak3, filter, CLASS_HPLEAK3);
DEFINE_SUBSTANCE(filter_Addantislow, filter, CLASS_ADDANTISLOW);
//145
DEFINE_SUBSTANCE(filter_Addweak, filter, CLASS_ADDWEAK);
DEFINE_SUBSTANCE(filter_Decdodge, filter, CLASS_DECDODGE);
DEFINE_SUBSTANCE(filter_Mpleak, filter, CLASS_MPLEAK);
DEFINE_SUBSTANCE(filter_Addspot, filter, CLASS_ADDSPOT);
DEFINE_SUBSTANCE(filter_Zhaoqi, filter, CLASS_ZHAOQI);
//150
DEFINE_SUBSTANCE(filter_Seed, filter, CLASS_SEED);
DEFINE_SUBSTANCE(filter_Subantisilent, filter, CLASS_SUBANTISILENT);
DEFINE_SUBSTANCE(filter_Tuoling2, filter, CLASS_TUOLING2);
DEFINE_SUBSTANCE(filter_Sleeptimer, filter, CLASS_SLEEPTIMER);
DEFINE_SUBSTANCE(filter_Miracleburstfo, filter, CLASS_MIRACLEBURSTFO);
//155
DEFINE_SUBSTANCE(filter_Hpgen, filter, CLASS_HPGEN);
DEFINE_SUBSTANCE(filter_Aswind, filter, CLASS_ASWIND);
DEFINE_SUBSTANCE(filter_Limitcyclearea, filter, CLASS_LIMITCYCLEAREA);
DEFINE_SUBSTANCE(filter_CatchPet, filter, CLASS_CATCHPET);
DEFINE_SUBSTANCE(filter_Permitcyclearea, filter, CLASS_PERMITCYCLEAREA);
//120
DEFINE_SUBSTANCE(filter_Subantidizzy2, filter, CLASS_SUBANTIDIZZY2);
DEFINE_SUBSTANCE(filter_Icon, filter, CLASS_ICON);
DEFINE_SUBSTANCE(filter_Fogstate, filter, CLASS_FOGSTATE);
DEFINE_SUBSTANCE(filter_Dodge, filter, CLASS_DODGE);
DEFINE_SUBSTANCE(filter_Hpleak4, filter, CLASS_HPLEAK4);
//125
DEFINE_SUBSTANCE(filter_Addantiwrap, filter, CLASS_ADDANTIWRAP);
DEFINE_SUBSTANCE(filter_Rmhumanform, filter, CLASS_RMHUMANFORM);
DEFINE_SUBSTANCE(filter_Ignite, filter, CLASS_IGNITE);
DEFINE_SUBSTANCE(filter_Diet, filter, CLASS_DIET);
DEFINE_SUBSTANCE(filter_Attack2hp, filter, CLASS_ATTACK2HP);
//130
DEFINE_SUBSTANCE(filter_Tuoling1, filter, CLASS_TUOLING1);
DEFINE_SUBSTANCE(filter_Incattack, filter, CLASS_INCATTACK);
DEFINE_SUBSTANCE(filter_Foxstate, filter, CLASS_FOXSTATE);
DEFINE_SUBSTANCE(filter_Holyaura, filter, CLASS_HOLYAURA);
DEFINE_SUBSTANCE(filter_Puppetskill, filter, CLASS_PUPPETSKILL);
//135
DEFINE_SUBSTANCE(filter_Inccritrate, filter, CLASS_INCCRITRATE);
DEFINE_SUBSTANCE(filter_Immuneslow, filter, CLASS_IMMUNESLOW);
DEFINE_SUBSTANCE(filter_Incmp, filter, CLASS_INCMP);
DEFINE_SUBSTANCE(filter_Barehanded, filter, CLASS_BAREHANDED);
DEFINE_SUBSTANCE(filter_Turnbuff, filter, CLASS_TURNBUFF);
//140
DEFINE_SUBSTANCE(filter_Seniordarkform, filter, CLASS_SENIORDARKFORM);
DEFINE_SUBSTANCE(filter_Decfatalratio, filter, CLASS_DECFATALRATIO);
DEFINE_SUBSTANCE(filter_Talismaneffectsbless, filter, CLASS_TALISMANEFFECTBLESS);
DEFINE_SUBSTANCE(filter_Posexchangewithclone, filter, CLASS_POSEXCHANGEWITHCLONE);
DEFINE_SUBSTANCE(filter_Gtaddattackdefense, filter, CLASS_GTADDATTACKDEFENSE);
//145
DEFINE_SUBSTANCE(filter_Subantisilent2, filter, CLASS_SUBANTISILENT2);
DEFINE_SUBSTANCE(filter_Scopedamage, filter, CLASS_SCOPEDAMAGE);
DEFINE_SUBSTANCE(filter_Skillmirror, filter, CLASS_SKILLMIRROR);
DEFINE_SUBSTANCE(filter_Addantisleep, filter, CLASS_ADDANTISLEEP);
DEFINE_SUBSTANCE(filter_Deathscatter, filter, CLASS_DEATHSCATTER);
//150
DEFINE_SUBSTANCE(filter_Spiritdrag, filter, CLASS_SPIRITDRAG);
DEFINE_SUBSTANCE(filter_Immunewrap, filter, CLASS_IMMUNEWRAP);
DEFINE_SUBSTANCE(filter_Turndebuff, filter, CLASS_TURNDEBUFF);
DEFINE_SUBSTANCE(filter_Retort, filter, CLASS_RETORT);
DEFINE_SUBSTANCE(filter_Asforest, filter, CLASS_ASFOREST);
//155
DEFINE_SUBSTANCE(filter_Addspeed, filter, CLASS_ADDSPEED);
DEFINE_SUBSTANCE(filter_Jail, filter, CLASS_JAIL);
DEFINE_SUBSTANCE(filter_Addrage2, filter, CLASS_ADDRAGE2);
DEFINE_SUBSTANCE(filter_Attack2mp, filter, CLASS_ATTACK2MP);
DEFINE_SUBSTANCE(filter_Immunedizzy, filter, CLASS_IMMUNEDIZZY);
//160
DEFINE_SUBSTANCE(filter_Buxiu, filter, CLASS_BUXIU);
DEFINE_SUBSTANCE(filter_Tiangu, filter, CLASS_TIANGU);
DEFINE_SUBSTANCE(filter_Dodgeregain, filter, CLASS_DODGEREGAIN);
DEFINE_SUBSTANCE(filter_Subantiwrap, filter, CLASS_SUBANTIWRAP);
DEFINE_SUBSTANCE(filter_Swift, filter, CLASS_SWIFT);
//165
DEFINE_SUBSTANCE(filter_Bloodthirsty, filter, CLASS_BLOODTHIRSTY);
DEFINE_SUBSTANCE(filter_Ashill, filter, CLASS_ASHILL);
DEFINE_SUBSTANCE(filter_Addhp2, filter, CLASS_ADDHP2);
DEFINE_SUBSTANCE(filter_Incmountspeed, filter, CLASS_INCMOUNTSPEED);
DEFINE_SUBSTANCE(filter_Recordpos, filter, CLASS_RECORDPOS);
//170
DEFINE_SUBSTANCE(filter_Addmp, filter, CLASS_ADDMP);
DEFINE_SUBSTANCE(filter_Subanti, filter, CLASS_SUBANTI);
DEFINE_SUBSTANCE(filter_Cycdecattack, filter, CLASS_CYCDECATTACK);
DEFINE_SUBSTANCE(filter_Gangdan, filter, CLASS_GANGDAN);
DEFINE_SUBSTANCE(filter_Fashen, filter, CLASS_FASHEN);
//175
DEFINE_SUBSTANCE(filter_Bloodthirsty2, filter, CLASS_BLOODTHIRSTY2);
DEFINE_SUBSTANCE(filter_Miracleburstxian, filter, CLASS_MIRACLEBURSTXIAN);
DEFINE_SUBSTANCE(filter_Magicshield, filter, CLASS_MAGICSHIELD);
DEFINE_SUBSTANCE(filter_Addanti, filter, CLASS_ADDANTI);
DEFINE_SUBSTANCE(filter_Incdefence, filter, CLASS_INCDEFENCE);
//180
DEFINE_SUBSTANCE(filter_Recoverhp, filter, CLASS_RECOVERYHP);
DEFINE_SUBSTANCE(filter_Addattack, filter, CLASS_ADDATTACK);
DEFINE_SUBSTANCE(filter_Guilin, filter, CLASS_GUILIN);
DEFINE_SUBSTANCE(filter_Decfatalhurt, filter, CLASS_DECFATALHURT);
DEFINE_SUBSTANCE(filter_Firing, filter, CLASS_FIRING);
//185
DEFINE_SUBSTANCE(filter_Incdrugeffect, filter, CLASS_INCDRUGEFFECT);
DEFINE_SUBSTANCE(filter_Decanti, filter, CLASS_DECANTI);
DEFINE_SUBSTANCE(filter_Lightuniform, filter, CLASS_LIGHTUNIFORM);
DEFINE_SUBSTANCE(filter_Bloodshield, filter, CLASS_BLOODSHIELD);
DEFINE_SUBSTANCE(filter_Incfatalhurt, filter, CLASS_INCFATALHURT);
//190
DEFINE_SUBSTANCE(filter_Inccrithurt, filter, CLASS_INCCRITHURT);
DEFINE_SUBSTANCE(filter_Dizzy, filter, CLASS_DIZZY);
DEFINE_SUBSTANCE(filter_Sleep, filter, CLASS_SLEEP);
DEFINE_SUBSTANCE(filter_Decattack, filter, CLASS_DECATTACK);
DEFINE_SUBSTANCE(filter_Wrap, filter, CLASS_WRAP);
//195
DEFINE_SUBSTANCE(filter_Slow, filter, CLASS_SLOW);
DEFINE_SUBSTANCE(filter_Silent, filter, CLASS_SILENT);
DEFINE_SUBSTANCE(filter_Dsleep, filter, CLASS_DSLEEP);
DEFINE_SUBSTANCE(filter_Subhide, filter, CLASS_SUBHIDE);
DEFINE_SUBSTANCE(filter_Incskilldodge, filter, CLASS_INCSKILLDODGE);
//200
DEFINE_SUBSTANCE(filter_Divinityburst, filter, CLASS_DIVINITYBURST);
DEFINE_SUBSTANCE(filter_Deadlybless, filter, CLASS_DEADLYBLESS);
DEFINE_SUBSTANCE(filter_Coldinjure, filter, CLASS_COLDINJURE);
DEFINE_SUBSTANCE(filter_Evilaura, filter, CLASS_EVILAURA);
DEFINE_SUBSTANCE(filter_Submp, filter, CLASS_SUBMP); 
//205
DEFINE_SUBSTANCE(filter_Addrage1, filter, CLASS_ADDRAGE1);
DEFINE_SUBSTANCE(filter_Mpgen, filter, CLASS_MPGEN);
DEFINE_SUBSTANCE(filter_Viphupo, filter, CLASS_VIPHUPO);
DEFINE_SUBSTANCE(filter_Befired, filter, CLASS_BEFIRED);
DEFINE_SUBSTANCE(filter_Fear, filter, CLASS_FEAR);
//210
DEFINE_SUBSTANCE(filter_Betaunted2, filter, CLASS_BETAUNTED2);
DEFINE_SUBSTANCE(filter_Hpleak5, filter, CLASS_HPLEAK5);
DEFINE_SUBSTANCE(filter_Subantisleep2, filter, CLASS_SUBANTISLEEP2);
DEFINE_SUBSTANCE(filter_Ghostform, filter, CLASS_GHOSTFORM);
DEFINE_SUBSTANCE(filter_Bloodpool, filter, CLASS_BLOODPOOL);
//215
DEFINE_SUBSTANCE(filter_Miracleburstmo, filter, CLASS_MIRACLEBURSTMO);
DEFINE_SUBSTANCE(filter_Seniorlightform, filter, CLASS_SENIORLIGHTFORM);
DEFINE_SUBSTANCE(filter_Hpleak, filter, CLASS_HPLEAK);
DEFINE_SUBSTANCE(filter_Decskilldamage, filter, CLASS_DECSKILLDAMAGE);
DEFINE_SUBSTANCE(filter_Gthpgen, filter, CLASS_GTHPGEN);
//220
DEFINE_SUBSTANCE(filter_Decmp, filter, CLASS_DECMP);
DEFINE_SUBSTANCE(filter_Powerup, filter, CLASS_POWERUP);
DEFINE_SUBSTANCE(filter_Shouyi, filter, CLASS_SHOUYI);
DEFINE_SUBSTANCE(filter_Incheal, filter, CLASS_INCHEAL);
DEFINE_SUBSTANCE(filter_Creditboost, filter, CLASS_CREDITBOOST);
//225
DEFINE_SUBSTANCE(filter_Hurtscatter, filter, CLASS_HURTSCATTER);
DEFINE_SUBSTANCE(filter_Incattack2, filter, CLASS_INCATTACK2);
DEFINE_SUBSTANCE(filter_Slow2, filter, CLASS_SLOW2);
DEFINE_SUBSTANCE(filter_Incdefence2, filter, CLASS_INCDEFENCE2);
DEFINE_SUBSTANCE(filter_Immunetransform, filter, CLASS_IMMUNETRANSFORM);
//230
DEFINE_SUBSTANCE(filter_Addantitransform, filter, CLASS_ADDANTITRANSFORM);
DEFINE_SUBSTANCE(filter_Subantitransform, filter, CLASS_SUBANTITRANSFORM);
DEFINE_SUBSTANCE(filter_Drawclonelife, filter, CLASS_DRAWCLONELIFE);
DEFINE_SUBSTANCE(filter_Backstab, filter, CLASS_BACKSTAB);
DEFINE_SUBSTANCE(filter_Critcurse, filter, CLASS_CRITCURSE);
//235
DEFINE_SUBSTANCE(filter_Subspot, filter, CLASS_SUBSPOT);
DEFINE_SUBSTANCE(filter_Addhide, filter, CLASS_ADDHIDE);
DEFINE_SUBSTANCE(filter_Crippled, filter, CLASS_CRIPPLED);
DEFINE_SUBSTANCE(filter_Guishen1debuff, filter, CLASS_GUISHEN1DEBUFF);
DEFINE_SUBSTANCE(filter_Tuoling1debuff, filter, CLASS_TUOLING1DEBUFF);
//240
DEFINE_SUBSTANCE(filter_Tuoling2debuff, filter, CLASS_TUOLING2DEBUFF);
DEFINE_SUBSTANCE(filter_Pzmaxspeed, filter, CLASS_PZMAXSPEED);
DEFINE_SUBSTANCE(filter_Pzhalfspeed, filter, CLASS_PZHALFSPEED);
DEFINE_SUBSTANCE(filter_Pzhill, filter, CLASS_PZHILL);
DEFINE_SUBSTANCE(filter_Pznomove, filter, CLASS_PZNOMOVE);
//245
DEFINE_SUBSTANCE(filter_Pzchaos, filter, CLASS_PZCHAOS);
DEFINE_SUBSTANCE(filter_Pzvoid, filter, CLASS_PZVOID);
DEFINE_SUBSTANCE(filter_Activebeattacked, filter, CLASS_ACTIVEBEATTACKED);
DEFINE_SUBSTANCE(filter_Darklaser, filter, CLASS_DARKLASER);
DEFINE_SUBSTANCE(filter_Dirstate, filter, CLASS_DIRSTATE);
//250
DEFINE_SUBSTANCE(filter_Showyb, filter, CLASS_SHOWYB);
DEFINE_SUBSTANCE(filter_Subantiweak2, filter, CLASS_SUBANTIWEAK2);
DEFINE_SUBSTANCE(filter_Subantislow2, filter, CLASS_SUBANTISLOW2);
DEFINE_SUBSTANCE(filter_Submp2, filter, CLASS_SUBMP2);
DEFINE_SUBSTANCE(filter_Renma1, filter, CLASS_RENMA1);
//255
DEFINE_SUBSTANCE(filter_Renma2, filter, CLASS_RENMA2);
DEFINE_SUBSTANCE(filter_Renma3, filter, CLASS_RENMA3);
DEFINE_SUBSTANCE(filter_Renma4, filter, CLASS_RENMA4);

///////////////////////////////skillWrapper
DEFINE_SUBSTANCE(filter_guiwang_addanticrit, filter, CLASS_GUIWANG_ADDANTICRIT);
DEFINE_SUBSTANCE(filter_guiwang_addmaxhp, filter, CLASS_GUIWANG_ADDMAXHP);
//260
DEFINE_SUBSTANCE(filter_guiwang_adddamage, filter, CLASS_GUIWANG_ADDDAMAGE);
DEFINE_SUBSTANCE(filter_guiwang_addskillattack, filter, CLASS_GUIWANG_ADDSKILLATTACK);
DEFINE_SUBSTANCE(filter_guiwang_addtenaciy, filter, CLASS_GUIWANG_ADDTENACIY);




//
//DEFINE_SUBSTANCE(filter_Incdefence ,filter,CLASS_INCDEFENCE);
//DEFINE_SUBSTANCE(filter_Incattack ,filter, CLASS_INCATTACK );
//DEFINE_SUBSTANCE(filter_Inchp ,filter,CLASS_INCHP);
//DEFINE_SUBSTANCE(filter_Incmp ,filter,CLASS_INCMP);
//DEFINE_SUBSTANCE(filter_Inchpgen ,filter,CLASS_INCHPGEN);
//DEFINE_SUBSTANCE(filter_Incmpgen ,filter,CLASS_INCMPGEN);
//DEFINE_SUBSTANCE(filter_Adddodge ,filter,CLASS_ADDDODGE);
//DEFINE_SUBSTANCE(filter_Weak ,filter,CLASS_WEAK);
//DEFINE_SUBSTANCE(filter_Slow ,filter,CLASS_SLOW);
//DEFINE_SUBSTANCE(filter_Sleep ,filter,CLASS_SLEEP);
//DEFINE_SUBSTANCE(filter_Dizzy ,filter,CLASS_DIZZY);
//DEFINE_SUBSTANCE(filter_Wrap ,filter,CLASS_WRAP);
//DEFINE_SUBSTANCE(filter_Crazy ,filter,CLASS_CRAZY);
//DEFINE_SUBSTANCE(filter_Dechurt ,filter,CLASS_DECHURT);
//DEFINE_SUBSTANCE(filter_Magicshield ,filter,CLASS_MAGICSHIELD);
//DEFINE_SUBSTANCE(filter_Hpgen ,filter,CLASS_HPGEN);
//DEFINE_SUBSTANCE(filter_Mpgen ,filter,CLASS_MPGEN);
//DEFINE_SUBSTANCE(filter_Addhp ,filter,CLASS_ADDHP);
//DEFINE_SUBSTANCE(filter_Addmp ,filter,CLASS_ADDMP);
//DEFINE_SUBSTANCE(filter_Addattack ,filter,CLASS_ADDATTACK);
//DEFINE_SUBSTANCE(filter_Adddefence ,filter,CLASS_ADDDEFENCE);
//DEFINE_SUBSTANCE(filter_Addantidizzy ,filter,CLASS_ADDANTIDIZZY);
//DEFINE_SUBSTANCE(filter_Addantisleep ,filter,CLASS_ADDANTISLEEP);
//DEFINE_SUBSTANCE(filter_Addantisilent ,filter,CLASS_ADDANTISILENT);
//DEFINE_SUBSTANCE(filter_Addantiweak ,filter,CLASS_ADDANTIWEAK);
//DEFINE_SUBSTANCE(filter_Addantiwrap ,filter,CLASS_ADDANTIWRAP);
//DEFINE_SUBSTANCE(filter_Addantislow ,filter,CLASS_ADDANTISLOW);
//DEFINE_SUBSTANCE(filter_Subantidizzy ,filter,CLASS_SUBANTIDIZZY);
//DEFINE_SUBSTANCE(filter_Subantisleep ,filter,CLASS_SUBANTISLEEP);
//DEFINE_SUBSTANCE(filter_Subantisilent ,filter,CLASS_SUBANTISILENT);
//DEFINE_SUBSTANCE(filter_Subantiweak ,filter,CLASS_SUBANTIWEAK);
//DEFINE_SUBSTANCE(filter_Subantiwrap ,filter,CLASS_SUBANTIWRAP);
//DEFINE_SUBSTANCE(filter_Subantislow,filter,CLASS_SUBANTISLOW);
//DEFINE_SUBSTANCE(filter_Addspeed,filter,CLASS_ADDSPEED);
//DEFINE_SUBSTANCE(filter_Silent,filter,CLASS_SILENT);
//DEFINE_SUBSTANCE(filter_Hpleak,filter,CLASS_HPLEAK);
//DEFINE_SUBSTANCE(filter_Barehanded,filter,CLASS_BAREHANDED);
//DEFINE_SUBSTANCE(filter_Dodge,filter,CLASS_DODGE);
//DEFINE_SUBSTANCE(filter_Inccritrate,filter,CLASS_INCCRITRATE);
//DEFINE_SUBSTANCE(filter_Subdefence,filter,CLASS_SUBDEFENCE);
//DEFINE_SUBSTANCE(filter_Decdefence,filter,CLASS_DECDEFENCE);
//DEFINE_SUBSTANCE(filter_Decdamage,filter,CLASS_DECDAMAGE);
//DEFINE_SUBSTANCE(filter_Immunedizzy,filter,CLASS_IMMUNEDIZZY);
//DEFINE_SUBSTANCE(filter_Immunesleep,filter,CLASS_IMMUNESLEEP);
//DEFINE_SUBSTANCE(filter_Immunesilent,filter,CLASS_IMMUNESILENT);
//DEFINE_SUBSTANCE(filter_Immuneweak,filter,CLASS_IMMUNEWEAK);
//DEFINE_SUBSTANCE(filter_Immunewrap,filter,CLASS_IMMUNEWRAP);
//DEFINE_SUBSTANCE(filter_Immuneslow,filter,CLASS_IMMUNESLOW);
//DEFINE_SUBSTANCE(filter_Diet,filter,CLASS_DIET);
//DEFINE_SUBSTANCE(filter_Inchurt,filter,CLASS_INCHURT);
//DEFINE_SUBSTANCE(filter_Blessed,filter,CLASS_BLESSED);
//DEFINE_SUBSTANCE(filter_Cursed,filter,CLASS_CURSED);
//DEFINE_SUBSTANCE(filter_Powerup,filter,CLASS_POWERUP);
//DEFINE_SUBSTANCE(filter_Bloodshield,filter,CLASS_BLOODSHIELD);
//DEFINE_SUBSTANCE(filter_Shouyi,filter,CLASS_SHOUYI);
//DEFINE_SUBSTANCE(filter_Ningjin,filter,CLASS_NINGJIN);
//DEFINE_SUBSTANCE(filter_Icon,filter,CLASS_INVINCIBLE);
//DEFINE_SUBSTANCE(filter_Damage2hp,filter,CLASS_DAMAGE2HP);
//DEFINE_SUBSTANCE(filter_Attack2hp,filter,CLASS_ATTACK2HP);
//DEFINE_SUBSTANCE(filter_Swift,filter,CLASS_SWIFT);
//DEFINE_SUBSTANCE(filter_Retort,filter,CLASS_RETORT);
//DEFINE_SUBSTANCE(filter_Sealed,filter,CLASS_SEALED);
//DEFINE_SUBSTANCE(filter_Subhp,filter,CLASS_SUBHP);
//DEFINE_SUBSTANCE(filter_Submp,filter,CLASS_SUBMP);
//DEFINE_SUBSTANCE(filter_Dechp ,filter,CLASS_DECHP);
//DEFINE_SUBSTANCE(filter_Decmp ,filter,CLASS_DECMP);
//DEFINE_SUBSTANCE(filter_Incanti,filter,CLASS_INCANTI);
//DEFINE_SUBSTANCE(filter_Decanti,filter,CLASS_DECANTI);
//DEFINE_SUBSTANCE(filter_Incdodge,filter,CLASS_INCDODGE);
//DEFINE_SUBSTANCE(filter_Decdodge,filter,CLASS_DECDODGE);
//DEFINE_SUBSTANCE(filter_Smiteattack,filter,CLASS_SMITEATTACK);
//DEFINE_SUBSTANCE(filter_Addhpgen ,filter,CLASS_ADDHPGEN);
//
//DEFINE_SUBSTANCE(filter_Shapeshift, base_filter, CLASS_SHAPESHIFT);
//DEFINE_SUBSTANCE(filter_Summon, base_filter, CLASS_SUMMON);
//DEFINE_SUBSTANCE(filter_Addfixhit, base_filter, CLASS_ADDFIXHIT);
//DEFINE_SUBSTANCE(filter_Decfixhit, base_filter, CLASS_DECFIXHIT);
//DEFINE_SUBSTANCE(filter_Addfixdamage, base_filter, CLASS_ADDFIXDAMAGE);
//DEFINE_SUBSTANCE(filter_Decfixdamage, base_filter, CLASS_DECFIXDAMAGE);
//DEFINE_SUBSTANCE(filter_Addscaledamage, base_filter, CLASS_ADDSCALEDAMAGE);
//DEFINE_SUBSTANCE(filter_Decscaledamage, base_filter, CLASS_DECSCALEDAMAGE);
//DEFINE_SUBSTANCE(filter_Addfixcritdamage, base_filter, CLASS_ADDFIXCRITDAMAGE);
//DEFINE_SUBSTANCE(filter_Decfixcritdamage, base_filter, CLASS_DECFIXCRITDAMAGE);
//DEFINE_SUBSTANCE(filter_Addfixspeed, base_filter, CLASS_ADDFIXSPEED);
//DEFINE_SUBSTANCE(filter_Decfixspeed, base_filter, CLASS_DECFIXSPEED);
//DEFINE_SUBSTANCE(filter_Addfixoverspeed, base_filter, CLASS_ADDFIXOVERSPEED);
//DEFINE_SUBSTANCE(filter_Decfixoverspeed, base_filter, CLASS_DECFIXOVERSPEED);
//DEFINE_SUBSTANCE(filter_Aura, base_filter, CLASS_AURA);
//DEFINE_SUBSTANCE(filter_Clearstate, base_filter, CLASS_CLEARSTATE);
//DEFINE_SUBSTANCE(filter_Genstate, base_filter, CLASS_GENSTATE);
//DEFINE_SUBSTANCE(filter_Castattachskill, base_filter, CLASS_CASTATTACHSKILL);
//DEFINE_SUBSTANCE(filter_Clrcooldown, base_filter, CLASS_CLRCOOLDOWN);
//DEFINE_SUBSTANCE(filter_Clrallcooldown, base_filter, CLASS_CLRALLCOOLDOWN);
//DEFINE_SUBSTANCE(filter_Setcooldown, base_filter, CLASS_SETCOOLDOWN);
//DEFINE_SUBSTANCE(filter_Hpleak2, base_filter, CLASS_HPLEAK2);
//DEFINE_SUBSTANCE(filter_Hpleak3, base_filter, CLASS_HPLEAK3);
//DEFINE_SUBSTANCE(filter_Newdiet, base_filter, CLASS_NEWDIET);
//DEFINE_SUBSTANCE(filter_Newsealed, base_filter, CLASS_NEWSEALED);
//DEFINE_SUBSTANCE(filter_Newhpgen, base_filter, CLASS_NEWHPGEN);
//DEFINE_SUBSTANCE(filter_Decfixmp, base_filter, CLASS_DECFIXMP);
//DEFINE_SUBSTANCE(filter_Addanticritdamage, base_filter, CLASS_ADDANTICRITDAMAGE);
//DEFINE_SUBSTANCE(filter_Decanticritdamage, base_filter, CLASS_DECANTICRITDAMAGE);
//
//DEFINE_SUBSTANCE(filter_Addhpfixlimit, base_filter, CLASS_ADDHPFIXLIMIT);
//DEFINE_SUBSTANCE(filter_Dechpfixlimit, base_filter, CLASS_DECHPFIXLIMIT);
//DEFINE_SUBSTANCE(filter_Addhpscalelimit, base_filter, CLASS_ADDHPSCALELIMIT);
//DEFINE_SUBSTANCE(filter_Newaddhpscalelimit, base_filter, CLASS_NEWADDHPSCALELIMIT);
//DEFINE_SUBSTANCE(filter_Dechpscalelimit, base_filter, CLASS_DECHPSCALELIMIT);
//DEFINE_SUBSTANCE(filter_Addmpfixlimit, base_filter, CLASS_ADDMPFIXLIMIT);
//DEFINE_SUBSTANCE(filter_Decmpfixlimit, base_filter, CLASS_DECMPFIXLIMIT);
//DEFINE_SUBSTANCE(filter_Addmpscalelimit, base_filter, CLASS_ADDMPSCALELIMIT);
//DEFINE_SUBSTANCE(filter_Decmpscalelimit, base_filter, CLASS_DECMPSCALELIMIT);
//DEFINE_SUBSTANCE(filter_Adddefencefix, base_filter, CLASS_ADDDEFENCEFIX);
//DEFINE_SUBSTANCE(filter_Decdefencefix, base_filter, CLASS_DECDEFENCEFIX);
//DEFINE_SUBSTANCE(filter_Adddefencescale, base_filter, CLASS_ADDDEFENCESCALE);
//DEFINE_SUBSTANCE(filter_Decdefencescale, base_filter, CLASS_DECDEFENCESCALE);
//DEFINE_SUBSTANCE(filter_Newresurrect, base_filter, CLASS_NEWRESURRECT);
//DEFINE_SUBSTANCE(filter_Transmission, base_filter, CLASS_TRANSMISSION);
//DEFINE_SUBSTANCE(filter_Genitem, base_filter, CLASS_GENITEM);
//DEFINE_SUBSTANCE(filter_Decfixcritrate, base_filter, CLASS_DECFIXCRITRATE);
//DEFINE_SUBSTANCE(filter_Addfixcritrate, base_filter, CLASS_ADDFIXCRITRATE);
//DEFINE_SUBSTANCE(filter_Newinvincible, base_filter, CLASS_NEWINVINCIBLE);
//DEFINE_SUBSTANCE(filter_Disinvincible, base_filter, CLASS_DISINVINCIBLE);
//DEFINE_SUBSTANCE(filter_Immuneinvincible, base_filter, CLASS_IMMUNEINVINCIBLE);
//DEFINE_SUBSTANCE(filter_Xiaoqiang, base_filter, CLASS_XIAOQIANG);
//DEFINE_SUBSTANCE(filter_Newdizzy, base_filter, CLASS_NEWDIZZY);
//DEFINE_SUBSTANCE(filter_Newsleep, base_filter, CLASS_NEWSLEEP);
//DEFINE_SUBSTANCE(filter_Newslow, base_filter, CLASS_NEWSLOW);
//DEFINE_SUBSTANCE(filter_Newweak, base_filter, CLASS_NEWWEAK);
//DEFINE_SUBSTANCE(filter_Newsilent, base_filter, CLASS_NEWSILENT);
//DEFINE_SUBSTANCE(filter_Newwrap, base_filter, CLASS_NEWWRAP);
//DEFINE_SUBSTANCE(filter_Modifydhr, base_filter, CLASS_MODIFYDHR);
//DEFINE_SUBSTANCE(filter_Summonnpc, base_filter, CLASS_SUMMONNPC);
//DEFINE_SUBSTANCE(filter_Aurad, base_filter, CLASS_AURAD);
//DEFINE_SUBSTANCE(filter_Addskillsummonnum, base_filter, CLASS_ADDSKILLSUMMONNUM);
//DEFINE_SUBSTANCE(filter_Decsummoncl, base_filter, CLASS_DECSUMMONCL);
//DEFINE_SUBSTANCE(filter_Addsummoncl, base_filter, CLASS_ADDSUMMONCL);
//DEFINE_SUBSTANCE(filter_Decsummonal, base_filter, CLASS_DECSUMMONAL);
//DEFINE_SUBSTANCE(filter_Addsummonal, base_filter, CLASS_ADDSUMMONAL);
//DEFINE_SUBSTANCE(filter_Decsummonbl, base_filter, CLASS_DECSUMMONBL);
//DEFINE_SUBSTANCE(filter_Addsummonbl, base_filter, CLASS_ADDSUMMONBL);
//DEFINE_SUBSTANCE(filter_Decsummondefrat, base_filter, CLASS_DECSUMMONDEFRAT);
//DEFINE_SUBSTANCE(filter_Addsummondefrat, base_filter, CLASS_ADDSUMMONDEFRAT);
//DEFINE_SUBSTANCE(filter_Decsummondmgrat, base_filter, CLASS_DECSUMMONDMGRAT);
//DEFINE_SUBSTANCE(filter_Addsummondmgrat, base_filter, CLASS_ADDSUMMONDMGRAT);
//DEFINE_SUBSTANCE(filter_Decsummonhprat, base_filter, CLASS_DECSUMMONHPRAT);
//DEFINE_SUBSTANCE(filter_Addsummonhprat, base_filter, CLASS_ADDSUMMONHPRAT);
//DEFINE_SUBSTANCE(filter_Retortskill, base_filter, CLASS_RETORTSKILL);
//DEFINE_SUBSTANCE(filter_Retortskill1, base_filter, CLASS_RETORTSKILL1);
//DEFINE_SUBSTANCE(filter_Zhanhun, base_filter, CLASS_ZHANHUN);
//DEFINE_SUBSTANCE(filter_Newretort, base_filter, CLASS_NEWRETORT);
//
//DEFINE_SUBSTANCE(filter_Newaddantidizzy, base_filter, CLASS_NEWADDANTIDIZZY);
//DEFINE_SUBSTANCE(filter_Newaddantisleep, base_filter, CLASS_NEWADDANTISLEEP);
//DEFINE_SUBSTANCE(filter_Newaddantiweak, base_filter, CLASS_NEWADDANTIWEAK);
//DEFINE_SUBSTANCE(filter_Newaddantisilent, base_filter, CLASS_NEWADDANTISILENT);
//DEFINE_SUBSTANCE(filter_Newaddantiwrap, base_filter, CLASS_NEWADDANTIWRAP);
//DEFINE_SUBSTANCE(filter_Newaddantislow, base_filter, CLASS_NEWADDANTISLOW);
//
//DEFINE_SUBSTANCE(filter_Newsubantidizzy, base_filter, CLASS_NEWSUBANTIDIZZY);
//DEFINE_SUBSTANCE(filter_Newsubantisleep, base_filter, CLASS_NEWSUBANTISLEEP);
//DEFINE_SUBSTANCE(filter_Newsubantiweak, base_filter, CLASS_NEWSUBANTIWEAK);
//DEFINE_SUBSTANCE(filter_Newsubantisilent, base_filter, CLASS_NEWSUBANTISILENT);
//DEFINE_SUBSTANCE(filter_Newsubantiwrap, base_filter, CLASS_NEWSUBANTIWRAP);
//DEFINE_SUBSTANCE(filter_Newsubantislow, base_filter, CLASS_NEWSUBANTISLOW);
//
//DEFINE_SUBSTANCE(filter_Newimmunedizzy, base_filter, CLASS_NEWIMMUNEDIZZY);
//DEFINE_SUBSTANCE(filter_Newimmunesleep, base_filter, CLASS_NEWIMMUNESLEEP);
//DEFINE_SUBSTANCE(filter_Newimmuneweak, base_filter, CLASS_NEWIMMUNEWEAK);
//DEFINE_SUBSTANCE(filter_Newimmunesilent, base_filter, CLASS_NEWIMMUNESILENT);
//DEFINE_SUBSTANCE(filter_Newimmunewrap, base_filter, CLASS_NEWIMMUNEWRAP);
//DEFINE_SUBSTANCE(filter_Newimmuneslow, base_filter, CLASS_NEWIMMUNESLOW);
//
//DEFINE_SUBSTANCE(filter_Sneak, base_filter, CLASS_SNEAK);
//DEFINE_SUBSTANCE(filter_Unsneak, base_filter, CLASS_UNSNEAK);
//DEFINE_SUBSTANCE(filter_Nosmite, base_filter, CLASS_NOSMITE);
//DEFINE_SUBSTANCE(filter_Sorcery, base_filter, CLASS_SORCERY);
//DEFINE_SUBSTANCE(filter_Reversalcontrol, base_filter, CLASS_REVERSALCONTROL);
//DEFINE_SUBSTANCE(filter_Newdecdamage, base_filter, CLASS_NEWDECDAMAGE);
//DEFINE_SUBSTANCE(filter_Newinchurt, base_filter, CLASS_NEWINCHURT);
//DEFINE_SUBSTANCE(filter_Transtotarget, base_filter, CLASS_TRANSTOTARGET);
//DEFINE_SUBSTANCE(filter_Modifyfaction, base_filter, CLASS_MODIFYFACTION);
//DEFINE_SUBSTANCE(filter_Modifyenemyfaction, base_filter, CLASS_MODIFYENEMYFACTION);
//DEFINE_SUBSTANCE(filter_Addskillatt, base_filter, CLASS_ADDSKILLATT);
//DEFINE_SUBSTANCE(filter_Addskillarm, base_filter, CLASS_ADDSKILLARM);
//DEFINE_SUBSTANCE(filter_Addfactdmgrat, base_filter, CLASS_ADDFACTDMGRAT);
//DEFINE_SUBSTANCE(filter_Addantifactdmgrat, base_filter, CLASS_ADDANTIFACTDMGRAT);
//DEFINE_SUBSTANCE(filter_Addfactdmg, base_filter, CLASS_ADDFACTDMG);
//DEFINE_SUBSTANCE(filter_Addantifactdmg, base_filter, CLASS_ADDANTIFACTDMG);
//DEFINE_SUBSTANCE(filter_Newheal, base_filter, CLASS_NEWHEAL);
//DEFINE_SUBSTANCE(filter_Xiuluoalliance, base_filter, CLASS_XIULUOALLIANCE);
//DEFINE_SUBSTANCE(filter_Jinglingalliance, base_filter, CLASS_JINGLINGALLIANCE);
//DEFINE_SUBSTANCE(filter_Jianlongassist, base_filter, CLASS_JIANLONGASSIST);
//DEFINE_SUBSTANCE(filter_Wangluoassist, base_filter, CLASS_WANGLUOASSIST);
//DEFINE_SUBSTANCE(filter_Jialongassist, base_filter, CLASS_JIALONGASSIST);
//DEFINE_SUBSTANCE(filter_Zhongcuiassist, base_filter, CLASS_ZHONGCUIASSIST);
//DEFINE_SUBSTANCE(filter_Jiaolongassist, base_filter, CLASS_JIAOLONGASSIST);
//DEFINE_SUBSTANCE(filter_Jumaassist, base_filter, CLASS_JUMAASSIST);
//DEFINE_SUBSTANCE(filter_Mpleak2, base_filter, CLASS_MPLEAK2);
//DEFINE_SUBSTANCE(filter_Clearstate2, base_filter, CLASS_CLEARSTATE2);
//DEFINE_SUBSTANCE(filter_Guishouassist, base_filter, CLASS_GUISHOUASSIST);
//DEFINE_SUBSTANCE(filter_Tianshibless, base_filter, CLASS_TIANSHIBLESS);
//DEFINE_SUBSTANCE(filter_Emobless, base_filter, CLASS_EMOBLESS);
//DEFINE_SUBSTANCE(filter_Adddizzyfouse, base_filter, CLASS_ADDDIZZYFOUSE);
//DEFINE_SUBSTANCE(filter_Addsleepfouse, base_filter, CLASS_ADDSLEEPFOUSE);
//DEFINE_SUBSTANCE(filter_Addweakfouse, base_filter, CLASS_ADDWEAKFOUSE);
//DEFINE_SUBSTANCE(filter_Addsilentfouse, base_filter, CLASS_ADDSILENTFOUSE);
//DEFINE_SUBSTANCE(filter_Addwrapfouse, base_filter, CLASS_ADDWRAPFOUSE);
//DEFINE_SUBSTANCE(filter_Addslowfouse, base_filter, CLASS_ADDSLOWFOUSE);
//DEFINE_SUBSTANCE(filter_Resetfaction, base_filter, CLASS_RESETFACTION);
//DEFINE_SUBSTANCE(filter_Resetenemyfaction, base_filter, CLASS_RESETENEMYFACTION);
//DEFINE_SUBSTANCE(filter_Hpleak4, base_filter, CLASS_HPLEAK4);
//DEFINE_SUBSTANCE(filter_Newdodge, base_filter, CLASS_NEWDODGE);
//DEFINE_SUBSTANCE(filter_Shapeshift2, base_filter, CLASS_SHAPESHIFT2);
//DEFINE_SUBSTANCE(filter_Summon2, base_filter, CLASS_SUMMON2);
//DEFINE_SUBSTANCE(filter_Addfixspeed2, base_filter, CLASS_ADDFIXSPEED2);
//DEFINE_SUBSTANCE(filter_Decfixhit2, base_filter, CLASS_DECFIXHIT2);
//DEFINE_SUBSTANCE(filter_Addfixdamage2, base_filter, CLASS_ADDFIXDAMAGE2);
//DEFINE_SUBSTANCE(filter_Decfixdamage2, base_filter, CLASS_DECFIXDAMAGE2);
//DEFINE_SUBSTANCE(filter_Addscaledamage2, base_filter, CLASS_ADDSCALEDAMAGE2);
//DEFINE_SUBSTANCE(filter_Decscaledamage2, base_filter, CLASS_DECSCALEDAMAGE2);
//DEFINE_SUBSTANCE(filter_Addfixcritdamage2, base_filter, CLASS_ADDFIXCRITDAMAGE2);
//DEFINE_SUBSTANCE(filter_Decfixcritdamage2, base_filter, CLASS_DECFIXCRITDAMAGE2);
//DEFINE_SUBSTANCE(filter_Addscaledamagereduce, base_filter, CLASS_ADDSCALEDAMAGEREDUCE);
//DEFINE_SUBSTANCE(filter_Decscaledamagereduce, base_filter, CLASS_DECSCALEDAMAGEREDUCE);
//DEFINE_SUBSTANCE(filter_Assistance1, base_filter, CLASS_ASSISTANCE1);
//DEFINE_SUBSTANCE(filter_Assistance2, base_filter, CLASS_ASSISTANCE2);
//DEFINE_SUBSTANCE(filter_Assistance3, base_filter, CLASS_ASSISTANCE3);
//DEFINE_SUBSTANCE(filter_Assistance4, base_filter, CLASS_ASSISTANCE4);
//DEFINE_SUBSTANCE(filter_Assistance5, base_filter, CLASS_ASSISTANCE5);
//DEFINE_SUBSTANCE(filter_Assistance6, base_filter, CLASS_ASSISTANCE6);
//DEFINE_SUBSTANCE(filter_Assistance7, base_filter, CLASS_ASSISTANCE7);
//DEFINE_SUBSTANCE(filter_Assistance8, base_filter, CLASS_ASSISTANCE8);
//DEFINE_SUBSTANCE(filter_Assistance9, base_filter, CLASS_ASSISTANCE9);
//DEFINE_SUBSTANCE(filter_Assistance10, base_filter, CLASS_ASSISTANCE10);
//DEFINE_SUBSTANCE(filter_Assistance11, base_filter, CLASS_ASSISTANCE11);
//DEFINE_SUBSTANCE(filter_Assistance12, base_filter, CLASS_ASSISTANCE12);
//DEFINE_SUBSTANCE(filter_Assistance13, base_filter, CLASS_ASSISTANCE13);
//DEFINE_SUBSTANCE(filter_Newbarehanded, base_filter, CLASS_NEWBAREHANDED);
//DEFINE_SUBSTANCE(filter_Immuneassault, base_filter, CLASS_IMMUNEASSAULT);
//DEFINE_SUBSTANCE(filter_Addholyhitrate, base_filter, CLASS_ADDHOLYHITRATE);
//DEFINE_SUBSTANCE(filter_Decholyhitrate, base_filter, CLASS_DECHOLYHITRATE);
//DEFINE_SUBSTANCE(filter_Addholyhitdamage, base_filter, CLASS_ADDHOLYHITDAMAGE);
//DEFINE_SUBSTANCE(filter_Decholyhitdamage, base_filter, CLASS_DECHOLYHITDAMAGE);
//DEFINE_SUBSTANCE(filter_Addsneakpoint, base_filter, CLASS_ADDSNEAKPOINT);
//DEFINE_SUBSTANCE(filter_Decsneakpoint, base_filter, CLASS_DECSNEAKPOINT);
//DEFINE_SUBSTANCE(filter_Addantisneakpoint, base_filter, CLASS_ADDANTISNEAKPOINT);
//DEFINE_SUBSTANCE(filter_Decantisneakpoint, base_filter, CLASS_DECANTISNEAKPOINT);
//DEFINE_SUBSTANCE(filter_Unselect, base_filter, CLASS_UNSELECT);
//DEFINE_SUBSTANCE(filter_Forbidselect, base_filter, CLASS_FORBIDSELECT);
//DEFINE_SUBSTANCE(filter_Adddmgmastery0, base_filter, CLASS_ADDDMGMASTERY0);
//DEFINE_SUBSTANCE(filter_Adddmgmastery1, base_filter, CLASS_ADDDMGMASTERY1);
//DEFINE_SUBSTANCE(filter_Adddmgmastery2, base_filter, CLASS_ADDDMGMASTERY2);
//DEFINE_SUBSTANCE(filter_Adddmgmastery3, base_filter, CLASS_ADDDMGMASTERY3);
//DEFINE_SUBSTANCE(filter_Adddmgresistance0, base_filter, CLASS_ADDDMGRESISTANCE0);
//DEFINE_SUBSTANCE(filter_Adddmgresistance1, base_filter, CLASS_ADDDMGRESISTANCE1);
//DEFINE_SUBSTANCE(filter_Adddmgresistance2, base_filter, CLASS_ADDDMGRESISTANCE2);
//DEFINE_SUBSTANCE(filter_Adddmgresistance3, base_filter, CLASS_ADDDMGRESISTANCE3);
//DEFINE_SUBSTANCE(filter_Addscaleantidmgreduce, base_filter, CLASS_ADDSCALEANTIDMGREDUCE);
//DEFINE_SUBSTANCE(filter_Decscaleantidmgreduce, base_filter, CLASS_DECSCALEANTIDMGREDUCE);
//DEFINE_SUBSTANCE(filter_Forcesneakstate, base_filter, CLASS_FORCESNEAKSTATE);
//DEFINE_SUBSTANCE(filter_Newclearbuff, base_filter, CLASS_NEWCLEARBUFF);
//DEFINE_SUBSTANCE(filter_Newrandcurse, base_filter, CLASS_NEWRANDCURSE);
//DEFINE_SUBSTANCE(filter_Addfixcritrate2, base_filter, CLASS_ADDFIXCRITRATE2);
//DEFINE_SUBSTANCE(filter_Newinchurt2, base_filter, CLASS_NEWINCHURT2);
//DEFINE_SUBSTANCE(filter_Hpleak5, base_filter, CLASS_HPLEAK5);
//DEFINE_SUBSTANCE(filter_Hpleak6, base_filter, CLASS_HPLEAK6);
//DEFINE_SUBSTANCE(filter_Newinchurt3, base_filter, CLASS_NEWINCHURT3);
//DEFINE_SUBSTANCE(filter_Shapeshift3, base_filter, CLASS_SHAPESHIFT3);
//DEFINE_SUBSTANCE(filter_Deliverskill, base_filter, CLASS_DELIVERSKILL);
//DEFINE_SUBSTANCE(filter_Newblessed, base_filter, CLASS_NEWBLESSED);
//DEFINE_SUBSTANCE(filter_Newdizzy1, base_filter, CLASS_NEWDIZZY1);
//DEFINE_SUBSTANCE(filter_Newdizzy2, base_filter, CLASS_NEWDIZZY2);
//DEFINE_SUBSTANCE(filter_Newsleep1, base_filter, CLASS_NEWSLEEP1);
//DEFINE_SUBSTANCE(filter_Newslow1, base_filter, CLASS_NEWSLOW1);
//DEFINE_SUBSTANCE(filter_Newweak1, base_filter, CLASS_NEWWEAK1);
//DEFINE_SUBSTANCE(filter_Newsilent1, base_filter, CLASS_NEWSILENT1);
//DEFINE_SUBSTANCE(filter_Newwrap1, base_filter, CLASS_NEWWRAP1);
//DEFINE_SUBSTANCE(filter_Blessedandmagicshield, base_filter, CLASS_BLESSEDANDMAGICSHIELD);
//DEFINE_SUBSTANCE(filter_Newblessed1, base_filter, CLASS_NEWBLESSED1);
//DEFINE_SUBSTANCE(filter_Deccooldown, base_filter, CLASS_DECCOOLDOWN);
//DEFINE_SUBSTANCE(filter_Castattachskill1, base_filter, CLASS_CASTATTACHSKILL1);
//DEFINE_SUBSTANCE(filter_Shapeshift3random, base_filter, CLASS_SHAPESHIFT3RANDOM);
//DEFINE_SUBSTANCE(filter_Assistance14, base_filter, CLASS_ASSISTANCE14);
//DEFINE_SUBSTANCE(filter_Assistance15, base_filter, CLASS_ASSISTANCE15);
//DEFINE_SUBSTANCE(filter_Assistance16, base_filter, CLASS_ASSISTANCE16);
//DEFINE_SUBSTANCE(filter_Assistance17, base_filter, CLASS_ASSISTANCE17);
//DEFINE_SUBSTANCE(filter_Assistance18, base_filter, CLASS_ASSISTANCE18);
//DEFINE_SUBSTANCE(filter_Movefallblood, base_filter, CLASS_MOVEFALLBLOOD);
//DEFINE_SUBSTANCE(filter_Addskillatt1, base_filter, CLASS_ADDSKILLATT1);
//DEFINE_SUBSTANCE(filter_Addfinalmodspeed, base_filter, CLASS_ADDFINALMODSPEED);
//DEFINE_SUBSTANCE(filter_Newdizzy3, base_filter, CLASS_NEWDIZZY3);
//DEFINE_SUBSTANCE(filter_Newsleep3, base_filter, CLASS_NEWSLEEP3);
//DEFINE_SUBSTANCE(filter_Newslow3, base_filter, CLASS_NEWSLOW3);
//DEFINE_SUBSTANCE(filter_Newweak3, base_filter, CLASS_NEWWEAK3);
//DEFINE_SUBSTANCE(filter_Newsilent3, base_filter, CLASS_NEWSILENT3);
//DEFINE_SUBSTANCE(filter_Newwrap3, base_filter, CLASS_NEWWRAP3);
//DEFINE_SUBSTANCE(filter_Newinvincible1, base_filter, CLASS_NEWINVINCIBLE1);
//DEFINE_SUBSTANCE(filter_Addscaledamage3, base_filter, CLASS_ADDSCALEDAMAGE3);
//DEFINE_SUBSTANCE(filter_Addfixcritdamage3, base_filter, CLASS_ADDFIXCRITDAMAGE3);
//DEFINE_SUBSTANCE(filter_Addsummonskillattack, base_filter, CLASS_ADDSUMMONSKILLATTACK);
//DEFINE_SUBSTANCE(filter_Decsummonskillattack, base_filter, CLASS_DECSUMMONSKILLATTACK);
