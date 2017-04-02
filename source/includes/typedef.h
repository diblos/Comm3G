/*
 * typedef.h
 *
 * Created: 2012-03-28 3:12:40 PM
 *  Author: Justin
 */ 


#ifndef TYPEDEF_H_
#define TYPEDEF_H_


/*! \name Usual Types
 */
//! @{
typedef unsigned char           Bool; //!< Boolean.
#ifndef __cplusplus
#if !defined(__bool_true_false_are_defined)
typedef unsigned char           bool; //!< Boolean.
#endif
#endif
//! @}


//_____ M A C R O S ________________________________________________________

/*! \name Usual Constants
 */
//! @{
#define DISABLE   0
#define ENABLE    1
#ifndef __cplusplus
#if !defined(__bool_true_false_are_defined)
#define false     0
#define true      1
#endif
#endif
#define PASS      0
#define FAIL      1
#define LOW       0
#define HIGH      1
//! @}



#endif /* TYPEDEF_H_ */