#include "Battery.h"

static uint8_t Battery=0;
uint8_t BatteryFlag=0;

uint8_t Battery_calculate(uint16_t Battery_ADC)
{
	uint8_t BatteryLevel;
	if(Battery_ADC>4160)return 100;
	if(Battery_ADC<3580)return 0;
	switch (Battery_ADC)
	{
	case 4160:BatteryLevel=100;break;
	case 4156:BatteryLevel=99;break;
	case 4152:BatteryLevel=99;break;
	case 4148:BatteryLevel=99;break;
	case 4144:BatteryLevel=98;break;
	case 4140:BatteryLevel=97;break;
	case 4136:BatteryLevel=97;break;
	case 4132:BatteryLevel=96;break;
	case 4128:BatteryLevel=96;break;
	case 4124:BatteryLevel=96;break;
	case 4120:BatteryLevel=95;break;
	case 4116:BatteryLevel=95;break;
	case 4112:BatteryLevel=94;break;
	case 4108:BatteryLevel=93;break;
	case 4104:BatteryLevel=93;break;
	case 4100:BatteryLevel=92;break;
	case 4096:BatteryLevel=92;break;
	case 4092:BatteryLevel=91;break;
	case 4088:BatteryLevel=90;break;
	case 4084:BatteryLevel=90;break;
	case 4080:BatteryLevel=90;break;//90
	case 4076:BatteryLevel=90;break;
	case 4072:BatteryLevel=89;break;
	case 4068:BatteryLevel=89;break;
	case 4064:BatteryLevel=89;break;
	case 4060:BatteryLevel=88;break;
	case 4056:BatteryLevel=88;break;
	case 4052:BatteryLevel=87;break;//87
	case 4048:BatteryLevel=87;break;
	case 4044:BatteryLevel=87;break;
	case 4040:BatteryLevel=86;break;
	case 4036:BatteryLevel=86;break;
	case 4032:BatteryLevel=85;break;//85
	case 4028:BatteryLevel=85;break;
	case 4024:BatteryLevel=85;break;
	case 4020:BatteryLevel=84;break;
	case 4016:BatteryLevel=84;break;
	case 4012:BatteryLevel=84;break;
	case 4008:BatteryLevel=83;break;
	case 4004:BatteryLevel=83;break;
	case 4000:BatteryLevel=83;break;
	case 3996:BatteryLevel=82;break;
	case 3992:BatteryLevel=82;break;
	case 3988:BatteryLevel=82;break;
	case 3984:BatteryLevel=81;break;
	case 3980:BatteryLevel=80;break;
	case 3976:BatteryLevel=80;break;
	case 3972:BatteryLevel=80;break;//80
	case 3968:BatteryLevel=80;break;
	case 3964:BatteryLevel=79;break;
	case 3960:BatteryLevel=79;break;
	case 3956:BatteryLevel=78;break;
	case 3952:BatteryLevel=78;break;
	case 3948:BatteryLevel=77;break;
	case 3944:BatteryLevel=77;break;
	case 3940:BatteryLevel=76;break;
	case 3936:BatteryLevel=75;break;
	case 3932:BatteryLevel=75;break;//75
	case 3928:BatteryLevel=74;break;
	case 3924:BatteryLevel=73;break;
	case 3920:BatteryLevel=73;break;
	case 3916:BatteryLevel=72;break;
	case 3912:BatteryLevel=71;break;
	case 3908:BatteryLevel=71;break;
	case 3904:BatteryLevel=70;break;
	case 3900:BatteryLevel=70;break;//70
	case 3896:BatteryLevel=70;break;
	case 3892:BatteryLevel=69;break;
	case 3888:BatteryLevel=68;break;
	case 3884:BatteryLevel=67;break;
	case 3880:BatteryLevel=66;break;
	case 3876:BatteryLevel=65;break;
	case 3872:BatteryLevel=65;break;//65
	case 3868:BatteryLevel=65;break;
	case 3864:BatteryLevel=64;break;
	case 3860:BatteryLevel=63;break;
	case 3856:BatteryLevel=62;break;
	case 3852:BatteryLevel=62;break;
	case 3848:BatteryLevel=61;break;
	case 3844:BatteryLevel=61;break;
	case 3840:BatteryLevel=60;break;//60
	case 3836:BatteryLevel=60;break;
	case 3832:BatteryLevel=59;break;
	case 3828:BatteryLevel=58;break;
	case 3824:BatteryLevel=57;break;
	case 3820:BatteryLevel=56;break;
	case 3816:BatteryLevel=55;break;
	case 3812:BatteryLevel=55;break;//55
	case 3808:BatteryLevel=54;break;
	case 3804:BatteryLevel=53;break;
	case 3800:BatteryLevel=52;break;
	case 3796:BatteryLevel=51;break;
	case 3792:BatteryLevel=50;break;//50
	case 3788:BatteryLevel=49;break;
	case 3784:BatteryLevel=48;break;
	case 3780:BatteryLevel=47;break;
	case 3776:BatteryLevel=46;break;
	case 3772:BatteryLevel=45;break;//45
	case 3768:BatteryLevel=44;break;
	case 3764:BatteryLevel=43;break;
	case 3760:BatteryLevel=42;break;//42
	case 3756:BatteryLevel=41;break;///41
	case 3752:BatteryLevel=40;break;//40
	case 3748:BatteryLevel=39;break;
	case 3744:BatteryLevel=38;break;
	case 3740:BatteryLevel=37;break;
	case 3736:BatteryLevel=36;break;
	case 3732:BatteryLevel=35;break;//35
	case 3733:BatteryLevel=34;break;
	case 3731:BatteryLevel=33;break;
	case 3730:BatteryLevel=32;break;
	case 3729:BatteryLevel=31;break;
	case 3728:BatteryLevel=30;break;//30
	case 3724:BatteryLevel=29;break;
	case 3720:BatteryLevel=28;break;
	case 3716:BatteryLevel=27;break;
	case 3712:BatteryLevel=26;break;
	case 3708:BatteryLevel=25;break;//25
	case 3704:BatteryLevel=24;break;
	case 3700:BatteryLevel=23;break;
	case 3696:BatteryLevel=22;break;
	case 3691:BatteryLevel=21;break;
	case 3692:BatteryLevel=20;break;///20
	case 3688:BatteryLevel=19;break;
	case 3684:BatteryLevel=18;break;
	case 3680:BatteryLevel=17;break;
	case 3676:BatteryLevel=16;break;
	case 3672:BatteryLevel=15;break;//15
	case 3668:BatteryLevel=14;break;
	case 3664:BatteryLevel=13;break;
	case 3660:BatteryLevel=12;break;//12
	case 3656:BatteryLevel=11;break;
	case 3652:BatteryLevel=10;break;//10
	case 3648:BatteryLevel=9;break;
	case 3644:BatteryLevel=8;break;//8
	case 3640:BatteryLevel=8;break;
	case 3636:BatteryLevel=7;break;
	case 3632:BatteryLevel=6;break;
	case 3628:BatteryLevel=5;break;//5
	case 3624:BatteryLevel=4;break;
	case 3620:BatteryLevel=4;break;
	case 3616:BatteryLevel=3;break;
	case 3612:BatteryLevel=3;break;//3
	case 3608:BatteryLevel=3;break;
	case 3604:BatteryLevel=3;break;
	case 3600:BatteryLevel=2;break;
	case 3596:BatteryLevel=2;break;
	case 3592:BatteryLevel=1;break;//1
	case 3588:BatteryLevel=1;break;
	case 3584:BatteryLevel=1;break;
	case 3580:BatteryLevel=0;break;//0
	}
	return BatteryLevel;
}

void Update_BatteryLevel(uint16_t Battery_ADC)
{
    static uint8_t BatteryLevel_Last[5], BatteryLevel_Init_Flag = 0,BatteryLevel_Index=0;
	uint16_t BatteryLevel=0;
    if (!BatteryLevel_Init_Flag) {
	for(uint8_t i=0;i<5;i++)BatteryLevel_Last[i]      = Battery_calculate(Battery_ADC * 4);
    BatteryLevel_Init_Flag = 1;
    }
    BatteryLevel_Last[BatteryLevel_Index]=Battery_calculate(Battery_ADC * 4);
	BatteryLevel_Index++;
	BatteryLevel_Index%=5;
	for(uint8_t i=0;i<5;i++)
	{
		BatteryLevel+=BatteryLevel_Last[i];
	}
	Battery=BatteryLevel/5;
}

uint8_t Get_BatteryLevel()
{
    return Battery;
}
