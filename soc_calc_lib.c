#define DEF_DBG_I
#define DEF_DBG_W
#define DEF_DBG_E
#include "debug.h"
#include "x_frame.h"

/*-----soc show mod.c
				\
				 +---discharge mode: dsg_soc_to_num
				  \
				   +----charge mode: chg_soc_to_num
				    \
					 +-----led show: soc_to_led_by_num
					  \
				 	   +---led show: soc_to_led_by_soc ----public(user api)
				   
-----for example: 4 led
	 soc	   num
	0-25%:	 	1
	26-50%:		2
	51-75%:		3
	76-100%		4
*/

#define SOC_LED_NUM		4		//if num is changed, you need to modify the function of soc_to_led_by_num.
#define SOC_LED_DELT	(100/SOC_LED_NUM)
#define SOC_DSG_MIN		5

static void soc_to_led_by_num(uint8_t num)
{
	//clear all led 
	const uint32_t close_led 	= 1;
	const uint32_t open_led 	= 0;

	hal_app_write(DRV_GPIO,(uint8_t *)&close_led,sizeof(uint32_t),IDX_GPIO_LED1,NULL);
	hal_app_write(DRV_GPIO,(uint8_t *)&close_led,sizeof(uint32_t),IDX_GPIO_LED2,NULL);
	hal_app_write(DRV_GPIO,(uint8_t *)&close_led,sizeof(uint32_t),IDX_GPIO_LED3,NULL);
	hal_app_write(DRV_GPIO,(uint8_t *)&close_led,sizeof(uint32_t),IDX_GPIO_LED4,NULL);
	//hal_app_write(DRV_GPIO,(uint8_t *)&close_led,sizeof(uint32_t),IDX_GPIO_LED5,NULL);
	
	//set all led
	//if(num > 4)		{hal_app_write(DRV_GPIO,(uint8_t *)&open_led,sizeof(uint32_t),IDX_GPIO_LED5,NULL);}	//light 4 led
	if(num > 3)		{hal_app_write(DRV_GPIO,(uint8_t *)&open_led,sizeof(uint32_t),IDX_GPIO_LED4,NULL);}	//light 4 led
	if(num > 2)		{hal_app_write(DRV_GPIO,(uint8_t *)&open_led,sizeof(uint32_t),IDX_GPIO_LED3,NULL);}	//light 3 led
	if(num > 1)		{hal_app_write(DRV_GPIO,(uint8_t *)&open_led,sizeof(uint32_t),IDX_GPIO_LED2,NULL);}	//light 2 led
	if(num > 0)		{hal_app_write(DRV_GPIO,(uint8_t *)&open_led,sizeof(uint32_t),IDX_GPIO_LED1,NULL);}	//light 1 led
	
	//DBG_I("soc_to_led*******num:%d",num);
}

static uint8_t dsg_soc_to_num(uint8_t soc)
{
	uint8_t num = 0;
	
	if(soc > SOC_DSG_MIN)
	{
		if(soc < 100)	{num = (soc-1)/SOC_LED_DELT + 1;}	//Ä¿µÄ£º25%ÏÔÊ¾Ò»¸ñ
		else			{num = soc/SOC_LED_DELT + 1;}
	}
	else
	{
		static uint8_t cnt = 0;
		cnt++;
		cnt = cnt%2;
		if(cnt)		{num = SOC_LED_NUM;}
		else		{num = 0;}
	}
	return num;
}

static uint8_t chg_soc_to_num(uint8_t soc)
{
	uint8_t num = 0;
	static uint8_t cnt = 0;
	
	if(soc < 100)	{num = (soc-1)/SOC_LED_DELT + 1;}
	else			{num = soc/SOC_LED_DELT + 1;}
	
	if(soc < 100)
	{
		cnt++;
		cnt = cnt%2;
		if(cnt == 0)		{num = num - 1;}
	}
	return num;
}

#define SOC_DSG_MODE	0
#define SOC_CHG_MODE	1
void soc_to_led_by_soc(uint8_t chg_dsg_mode, uint8_t soc)
{
	uint8_t num = 0;
	if(chg_dsg_mode == SOC_DSG_MODE)
	{
		num = dsg_soc_to_num(soc);
	}
	else if(chg_dsg_mode == SOC_CHG_MODE)
	{
		num = chg_soc_to_num(soc);
	}
	
	soc_to_led_by_num(num);
}

//--------------------
typedef struct
{
	uint8_t		soc;
	float		vol;
} vol_to_soc_tab_t;

const vol_to_soc_tab_t vol_to_soc_tab[] =
{
    { 0    ,   3000  },	//vol <= 3.00,soc = 0
    { 1    ,   3120  },	//vol > 3.00,vol <= 3.12,soc = 1
    { 2    ,   3208  },
    { 3    ,   3269  },
    { 4    ,   3308  },
    { 5    ,   3327  },
    { 6    ,   3344  },
    { 7    ,   3358  },
    { 8    ,   3370  },
    { 9    ,   3381  },
    { 10    ,  3391   },
    { 11    ,  3403   },
    { 12    ,  3418   },
    { 13    ,  3433   },
    { 14    ,  3448   },
    { 15    ,  3461   },
    { 16    ,  3474   },
    { 17    ,  3487   },
    { 18    ,  3499   },
    { 19    ,  3510   },
    { 20    ,  3519   },
    { 21    ,  3529   },
    { 22    ,  3539   },
    { 23    ,  3548   },
    { 24    ,  3558   },
    { 25    ,  3565   },
    { 26    ,  3573   },
    { 27    ,  3581   },
    { 28    ,  3589   },
    { 29    ,  3597   },
    { 30    ,  3603   },
    { 31    ,  3610   },
    { 32    ,  3618   },
    { 33    ,  3626   },
    { 34    ,  3634   },
    { 35    ,  3642   },
    { 36    ,  3650   },
    { 37    ,  3659   },
    { 38    ,  3670   },
    { 39    ,  3681   },
    { 40    ,  3690   },
    { 41    ,  3702   },
    { 42    ,  3714   },
    { 43    ,  3725   },
    { 44    ,  3737   },
    { 45    ,  3747   },
    { 46    ,  3757   },
    { 47    ,  3767   },
    { 48    ,  3777   },
    { 49    ,  3786   },
    { 50    ,  3792   },
    { 51    ,  3800   },
    { 52    ,  3807   },
    { 53    ,  3815   },
    { 54    ,  3824   },
    { 55    ,  3836   },
    { 56    ,  3839   },
    { 57    ,  3845   },
    { 58    ,  3857   },
    { 59    ,  3861   },
    { 60    ,  3864   },
    { 61    ,  3870   },
    { 62    ,  3877   },
    { 63    ,  3884   },
    { 64    ,  3891   },
    { 65    ,  3895   },
    { 66    ,  3902   },
    { 67    ,  3909   },
    { 68    ,  3916   },
    { 69    ,  3925   },
    { 70    ,  3933   },
    { 71    ,  3942   },
    { 72    ,  3951   },
    { 73    ,  3960   },
    { 74    ,  3971   },
    { 75    ,  3978   },
    { 76    ,  3987   },
    { 77    ,  3994   },
    { 78    ,  4001   },
    { 79    ,  4007   },
    { 80    ,  4010   },
    { 81    ,  4014   },
    { 82    ,  4018   },
    { 83    ,  4021   },
    { 84    ,  4025   },
    { 85    ,  4028   },
    { 86    ,  4033   },
    { 87    ,  4039   },
    { 88    ,  4045   },
    { 89    ,  4052   },
    { 90    ,  4059   },
    { 91    ,  4066   },
    { 92    ,  4072   },
    { 93    ,  4077   },
    { 94    ,  4083   },
    { 95    ,  4088   },
    { 96    ,  4094   },
    { 97    ,  4101   },
    { 98    ,  4110   },
    { 99    ,  4122   },
    { 100   ,  4145   },
};

static uint8_t calc_vol_to_soc_val(uint16_t cur_vol, vol_to_soc_tab_t *p_vol_to_soc_tab)
{
	uint8_t i	= 0;
	uint8_t soc = 0;

	for(i = 0; i < ARRY_ITEMS_NUM(vol_to_soc_tab); i++)
	{
		if(cur_vol <= p_vol_to_soc_tab[i].vol)
		{
		    soc = p_vol_to_soc_tab[i].soc;
			return soc;
		}
		else										// over max voltage value, soc = 100
        {
            soc = 100;
        }
	}
	return soc;
}
uint8_t get_ocv_soc(uint16_t cur_vol)
{
	return calc_vol_to_soc_val(cur_vol,(vol_to_soc_tab_t*)&vol_to_soc_tab);
}
