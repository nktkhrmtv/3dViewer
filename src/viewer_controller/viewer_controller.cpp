#include "viewer_controller.h"

namespace s21 {

/**
 * @brief Конструктор контроллера, инициализирует модель представления.
 * @param Model Указатель на объект модели, который будет использоваться
 * контроллером.
 */
ViewerController::ViewerController(ViewerModel *Model) : viewer_model(Model){};

/**
 * @brief Загрузка объекта из файла формата OBJ.
 * @param filePath Путь к файлу OBJ, который необходимо загрузить.
 */
void ViewerController::Model_loadOBJ(const QString &filePath) {
  viewer_model->loadOBJ(filePath);
}

/**
 * @brief Перемещение фигуры на основе заданного действия
 * @param translateAct Тип действия для перемещения.
 * @param translateValue Значение перемещения.
 */
void ViewerController::modelTranslateFigure(
    const translateAction_t &translateAct, float translateValue) {
  viewer_model->translateFigure(translateAct, translateValue);
}

/**
 * @brief Поворот фигуры вокруг заданной оси.
 * @param rotateAct Тип действия для поворота (например, вокруг оси X, Y или Z).
 * @param rotateValue Угол поворота.
 */
void ViewerController::modelRotateAxis(const rotateAction_t &rotateAct,
                                       float rotateValue) {
  viewer_model->rotateAxis(rotateAct, rotateValue);
}

/**
 * @brief Масштабирование фигуры.
 * @param scaleF Тип масштаба.
 * @param scaleValue Значение масштаба.
 */
void ViewerController::modelScaleFigure(const ScaleType_t &scaleF,
                                        float scaleValue) {
  viewer_model->scaleFigure(scaleF, scaleValue);
}

/**
 * @brief Установка типа проекции модели.
 * @param prog_type Тип проекции.
 */
void ViewerController::modelSetProjection(const ProjectionType_t &prog_type) {
  viewer_model->setProjection(prog_type);
}

/**
 * @brief Установка модели по умолчанию с определенным флагом.
 * @param flag Флаг, определяющий действия по установке модели по умолчанию.
 */
void ViewerController::modelSetDefault(int flag) {
  viewer_model->setDefault(flag);
}

/**
 * @brief Установка цвета ребра модели.
 * @param FacetColor Цвет ребра, задаваемый пользователем.
 */
void ViewerController::modelSetFacetColor(const QColor &FacetColor) {
  viewer_model->setFacetColor(FacetColor);
}

/**
 * @brief Установка ширины ребра модели.
 * @param scaleF Тип масштаба для изменения ширины ребра.
 */
void ViewerController::modelSetFacetWidth(const ScaleType_t &scaleF) {
  viewer_model->setFacetWidth(scaleF);
}

/**
 * @brief Установка цвета вершины модели.
 * @param VerticeColor Цвет вершины.
 */
void ViewerController::modelSetVerticeColor(const QColor &VerticeColor) {
  viewer_model->setVerticeColor(VerticeColor);
}

/**
 * @brief Установка толщины вершины.
 * @param scaleF Тип масштаба для изменения.
 */
void ViewerController::modelSetVerticeWidth(const ScaleType_t &scaleF) {
  viewer_model->setVerticeWidth(scaleF);
}

/**
 * @brief Установка типа вершин модели.
 * @param verticeType Тип вершин.
 */
void ViewerController::modelSetVerticeType(const VerticeType_t &verticeType) {
  viewer_model->setVerticeType(verticeType);
}

/**
 * @brief Установка цвета фона.
 * @param backGroundColor Цвет фона.
 */
void ViewerController::modelSetBackGroundColor(const QColor &backGroundColor) {
  viewer_model->setBackGroundColor(backGroundColor);
}

/**
 * @brief Обработка движения мыши, при изменении положения кнопки.
 * @param delta Разница в координатах мыши.
 */
void ViewerController::modelMouseButtonMove(QPoint delta) {
  viewer_model->MouseButtonMove(delta);
}

/**
 * @brief Обработка прокрутки колесика мыши.
 * @param delta Разница в координатах прокрутки.
 */
void ViewerController::modelMouseWheelMove(QPoint delta) {
  viewer_model->MouseWheelMove(delta);
}

/**
 * @brief Преобразование строки в число с плавающей точкой.
 * @param inputText Входная строка, содержащая число.
 * @return Преобразованное число с плавающей точкой.
 */
float ViewerController::modelMakeFloat(QString inputText) {
  return viewer_model->makeFloat(inputText);
}

/**
 * @brief Получение списка вершин модели.
 * @return Вектор вершин модели.
 */
std::vector<QVector3D> ViewerController::modelGetVertices() {
  return viewer_model->getVertices();
}

/**
 * @brief Получение списка ребер модели.
 * @return Вектор ребер модели.
 */
std::vector<unsigned int> ViewerController::modelGetFacets() {
  return viewer_model->getFacets();
}

/**
 * @brief Получение аффинного преобразования модели.
 * @return Структура с аффинным преобразованием.
 */
AffineTransform_t ViewerController::modelGetAffineTransform() {
  return viewer_model->getAffineTransform();
}

/**
 * @brief Получение определения модели.
 * @return Структура с определением модели.
 */
ModelDefinition_t ViewerController::modelGetModelDefinition() {
  return viewer_model->getModelDefinition();
}

}  // namespace s21