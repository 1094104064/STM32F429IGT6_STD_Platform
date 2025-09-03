/**
  ******************************************************************************
  *
  * @file    main.c
  * @author  Jamin
  * @brief   Platform based on STM32F429IGT6
  *
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  **/


/*********************
 *      INCLUDES
 *********************/
#include "main.h"
#include "app_init.h"
/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  GLOBAL PROTOTYPES
 **********************/ 

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void Init(void);
static void Loop(void);
/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/ 

/**
  * @brief  Program entrance.
  * @param  None
  * @retval 0 Exit status of the program
  */
int main(void)
{
    Init();
    Loop();
    
    return 0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/**
  * @brief  All initialisation.
  * @param  None
  * @retval NULL
  */
static void Init(void)
{
    /*!< interrupt priority divide into groups */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	

    app_init_core();
    app_init_bsp();
    app_init_middleware();



    // vTaskStartScheduler();
}



/**
  * @brief  Program loop.
  * @param  None
  * @retval NULL
  */
static void Loop(void)
{
    for(;;) {

    }
}




/******************************* (END OF FILE) *********************************/

   
