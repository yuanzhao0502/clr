/*
 * io.h
 *
 *  Created on: Feb 17, 2014
 *      Author: mac-97-41
 */

#ifndef IO_H_
#define IO_H_
#include <clr_structs.h>
#include<metis_struct.h>

//Declaration of s
CompClrGraph* convert_Matrix_To_Comp (MatrixClrGraph* mGraph);
MatrixClrGraph* conver_Comp_To_Matrix(CompClrGraph* cGraph);


#endif /* IO_H_ */
