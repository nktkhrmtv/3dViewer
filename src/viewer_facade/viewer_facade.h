#ifndef VIEWER_FACADE
#define VIEWER_FACADE

#include "../viewer_view/viewer_view.h"

namespace s21 {

/**
 * @brief Класс, инкапсулирующий в себе паттерн MVC
 */
class ViewerFacade {
 public:
  ViewerFacade();
  ~ViewerFacade();
  void startViewer();

 private:
  ViewerModel* viewerModel;
  ViewerController* viewerController;
  MainWindow* mainWindow;
};

}  // namespace s21

#endif