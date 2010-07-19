#include "XYZFileIO.h"

XYZFileIO::XYZFileIO()
{
}

XYZFileIO::~XYZFileIO()
{
}

XYZMesh* XYZFileIO::newMeshFromFile(const string &fileName)
{
	XYZMesh *mesh;

	ifstream inFile;
	inFile.open(fileName.c_str(), ios::in | ios::binary);
	if (inFile.is_open())
	{	
		
		XYZHeader header;

		//See if we're using the new format or old format
		//if the first part of the file is ascii, then proceed as old format
		if (inFile.get() == 'i'){
			//Get width and height			
			//Move to after ascii text
			inFile.seekg(28);

			//Get image width and height; i.e. "640 x 480"
			inFile >> header.meshWidth;
			inFile.ignore(3);
			inFile >> header.meshHeight;

			//make sure we read the height correctly
			clog << header.meshWidth << " x " << header.meshHeight << endl;
			
			//Set the version to 1, because only the header is different from version 1
			header.version = 1; 
		} else {
			//Otherwise, read the header as the binary
			//	Read in the header from the file
			inFile.seekg(0);
			inFile.read(reinterpret_cast<char *>(&header), sizeof(header));
		}
		
		//Reads both version 1 and old format
		if(header.version == 1)
		{
			//	Read in a version 1 file
			int meshSize = header.meshWidth * header.meshHeight;

			glm::vec3 *vertices = new glm::vec3[meshSize];
			TextureVertex *textureVertices = new TextureVertex[meshSize];
			unsigned char *valid = new unsigned char[meshSize];

			inFile.read(reinterpret_cast<char *>(vertices), sizeof(glm::vec3) * meshSize);
			inFile.read(reinterpret_cast<char *>(textureVertices), sizeof(TextureVertex) * meshSize);
			inFile.read(reinterpret_cast<char *>(valid), sizeof(unsigned char) *meshSize);

			XYZPoint *points = new XYZPoint[meshSize];
			
			for(int point = 0; point < meshSize; point++)
			{
				points[point].vertex = vertices[point];
				points[point].textureVertex = textureVertices[point];
				//	If the point is valid then store 255 otherwise store 0
				points[point].valid = valid[point] != 0 ? 255 : 0;
			}

			//	Clean up our arrays now that we are done with them
			delete [] vertices;
			delete [] textureVertices;
			delete [] valid;

			mesh = new XYZMesh(header.meshWidth, header.meshHeight, points);
		}
		else
		{
			cout << "Unable to read version: " << header.version << " xyz file";
		}

		inFile.close();
	}
	else
	{
		cout << "Unable to open file: " << fileName.c_str() << endl;
	}

	return mesh;
}