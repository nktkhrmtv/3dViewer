#include "viewer_facade.h"

namespace s21 {

/**
 * @brief Конструктор класса ViewerFacade
 */
ViewerFacade::ViewerFacade() {
  viewerModel = new ViewerModel;
  viewerController = new ViewerController(viewerModel);
  mainWindow = new MainWindow(viewerController);
}

/**
 * @brief Деструктор класса ViewerModel
 */
ViewerFacade::~ViewerFacade() {
  delete viewerModel;
  delete viewerController;
  delete mainWindow;
}

/**
 * @brief Запуск приложения
 */
void ViewerFacade::startViewer() { mainWindow->show(); }

}  // namespace s21