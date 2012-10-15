#include "FileOutputStream.h"

void FileOutputStream::Open(void)
{
	m_io.openSaveStream("D:/Temp.avi", 576, 576, 30);
}

void FileOutputStream::WriteStream(shared_ptr<MeshInterchange> mesh)
{
	m_io.saveStream(*mesh.get());
}

void FileOutputStream::Close(void)
{
	m_io.closeSaveStream();
}