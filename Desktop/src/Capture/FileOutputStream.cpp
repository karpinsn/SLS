#include "FileOutputStream.h"

FileOutputStream::FileOutputStream(string& filename, int width, int height)
{
	m_filename = filename;
	m_width = width;
	m_height = height;
}

void FileOutputStream::Open(void)
{
	m_io.openSaveStream(m_filename, m_width, m_height, 30);
}

void FileOutputStream::WriteStream(shared_ptr<MeshInterchange> mesh)
{
	m_io.saveStream(*mesh.get());
}

void FileOutputStream::Close(void)
{
	m_io.closeSaveStream();
}