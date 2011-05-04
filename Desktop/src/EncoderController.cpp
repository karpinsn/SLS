#include "EncoderController.h"

EncoderController::EncoderController(QWidget* parent) : QWidget(parent)
{
}

EncoderController::~EncoderController()
{
}

void EncoderController::init(void)
{
  OpenGLWidget *glWidget = findChild<OpenGLWidget*>(QString::fromUtf8("encoderGLWidget"));
  glWidget->setGLContext(&m_encoder);
}

void EncoderController::exportCurrentFrame(void)
{
  QString fileName = QFileDialog::getSaveFileName(this, "Save File", "/", "Images (*.png)");

  if(!fileName.isEmpty())
  {
    ImageIO io;
    Texture holoimage = m_encoder.encode();
    io.saveTexture(fileName.toAscii().constData(), holoimage);
  }
}

void EncoderController::exportEntireVideo(QListWidget* fileList)
{
  Logger::logDebug("EncoderController - exportEntireVideo: Enter");

  QString fileName = QFileDialog::getSaveFileName(this, "Save File", "/", "Video (*.avi)");

  if(!fileName.isEmpty())
  {
      ImageIO io;
      bool canSaveFile = io.saveAviFile(fileName.toAscii().constData(), 512, 512, 30);

      if(canSaveFile)
      {
          XYZFileIO fileIO;

          //	Inform the user of the progress
          QProgressDialog progress("Encoding frames...", 0, 0, fileList->count(), this);

          Logger::logDebug("EncoderController - exportEntireVideo: Encoding frames");
          for(int itemNumber = 0; itemNumber < fileList->count(); itemNumber++)
          {
              //	Increase the progress
              progress.setValue(itemNumber);

              QListWidgetItem *item = fileList->item(itemNumber);

              AbstractMesh* currentMesh = fileIO.newMeshFromFile(item->text().toAscii().constData());
              m_encoder.setCurrentMesh(currentMesh);

              Texture holoimage = m_encoder.encode();
              io.saveAviFileWriteFrame(holoimage);
          }

          //	Last one done!
          progress.setValue(fileList->count());

          Logger::logDebug("EncoderController - exportEntireVideo: Encoding complete!");
          io.saveAviFileFinish();
      }
  }
}

void EncoderController::selectXYZM(const string &filename)
{
  XYZFileIO fileIO;
  AbstractMesh* currentMesh = fileIO.newMeshFromFile(filename);
  m_encoder.setCurrentMesh(currentMesh);
}

void EncoderController::_updateGL(void)
{
  Logger::logDebug("ViewController - _updateGL: Enter");

  OpenGLWidget* glContext = findChild<OpenGLWidget*>(QString::fromUtf8("encoderGLWidget"));

  if(NULL != glContext)
  {
    glContext->updateScene();
  }
  else
  {
    Logger::logError("ViewController - _updateGL: Unable to find OpenGL Widget");
  }
}
