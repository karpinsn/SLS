/**
 @file
 @author Nikolaus Karpinsky & William Lohry
 @since	09/30/10 
 */

#ifndef _XYZ_FILE_IO_H_
#define _XYZ_FILE_IO_H_

#include <memory>
#include <iostream>
#include <fstream>

#include "AbstractMesh.h"
#include "XYZMesh.h"

using namespace std;

/**
* Header in the second version of the XYZ file
*/
struct XYZHeader
{
	int version;
	int meshWidth;
	int meshHeight;
};

class XYZFileIO
{
public:
	/**
	* Reads the in the specified XYZ file and returns it as an 
	* abstract mesh. Caller assumes ownership of the mesh and 
	* needs to delete when nessasary
	*
	* @param fileName Filename of the XYZ file to read
	*
	* @return AbstractMesh of the XYZ file
	*/
	AbstractMesh* newMeshFromFile(const string &fileName);
};

#endif	// _XYZ_FILE_IO_H_