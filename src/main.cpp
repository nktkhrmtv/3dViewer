#include "viewer_facade/viewer_facade.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  s21::ViewerFacade viewerFacade;
  viewerFacade.startViewer();
  return a.exec();
}