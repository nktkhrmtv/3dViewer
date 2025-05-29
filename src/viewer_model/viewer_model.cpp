#include "viewer_model.h"

namespace s21 {

/**
 * @brief Перемещение фигуры по осям X, Y или Z
 *
 * @param translateAct Действие перемещения (например, translateXPlus,
 * translateYMinus)
 * @param translateValue Значение перемещения (если 0.0, используется значение
 * по умолчанию 0.5)
 */
void ViewerModel::translateFigure(const translateAction_t &translateAct,
                                  float translateValue) {
  if (translateValue == 0.0) translateValue = 0.5f;
  switch (translateAct) {
    case translateXPlus:
      affine_transform.translateX += translateValue;
      break;
    case translateXMinus:
      affine_transform.translateX -= translateValue;
      break;
    case translateYPlus:
      affine_transform.translateY += translateValue;
      break;
    case translateYMinus:
      affine_transform.translateY -= translateValue;
      break;
    case translateZPlus:
      affine_transform.translateZ += translateValue;
      break;
    case translateZMinus:
      affine_transform.translateZ -= translateValue;
      break;
    default:
      break;
  }
}

/**
 * @brief Вращение фигуры вокруг осей X, Y или Z
 *
 * @param rotateAct Действие вращения (например, rotateXPlus, rotateYMinus)
 * @param rotateValue Угол вращения (если 0.0, используется значение по
 * умолчанию 15.0)
 */
void ViewerModel::rotateAxis(const rotateAction_t &rotateAct,
                             float rotateValue) {
  if (rotateValue == 0.0) rotateValue = 15.0f;
  switch (rotateAct) {
    case rotateXPlus:
      affine_transform.rotateAngleX += rotateValue;
      break;
    case rotateXMinus:
      affine_transform.rotateAngleX -= rotateValue;
      break;
    case rotateYPlus:
      affine_transform.rotateAngleY += rotateValue;
      break;
    case rotateYMinus:
      affine_transform.rotateAngleY -= rotateValue;
      break;
    case rotateZPlus:
      affine_transform.rotateAngleZ += rotateValue;
      break;
    case rotateZMinus:
      affine_transform.rotateAngleZ -= rotateValue;
      break;
    default:
      break;
  }
}

/**
 * @brief Установка типа проекции (параллельная или перспективная)
 *
 * @param prog_type Тип проекции (Parallel или Perspective)
 */
void ViewerModel::setProjection(const ProjectionType_t &prog_type) {
  if (prog_type == Parallel)
    affine_transform.projectionType = Parallel;
  else
    affine_transform.projectionType = Perspective;
}

/**
 * @brief Масштабирование фигуры
 *
 * @param scaleF Тип масштабирования (scalePlus или scaleMinus)
 * @param scaleValue Значение масштабирования (если 0.0, используется значение
 * по умолчанию)
 */
void ViewerModel::scaleFigure(const ScaleType_t &scaleF, float scaleValue) {
  if (affine_transform.projectionType == Perspective) {
    float delta_fov;  // Шаг изменения FOV
    if (scaleValue == 0)
      delta_fov = 3.0f;
    else
      delta_fov = scaleValue;
    if (scaleF == scalePlus) {
      affine_transform.fov -=
          delta_fov;  // Уменьшение FOV (увеличение масштаба)
    } else {
      affine_transform.fov +=
          delta_fov;  // Увеличение FOV (уменьшение масштаба)
    }
    // Ограничение FOV
    if (affine_transform.fov < 10.0f) affine_transform.fov = 10.0f;
    if (affine_transform.fov > 120.0f) affine_transform.fov = 120.0f;
  } else {
    float delta_scale;  // Масштабирование для параллельной проекции
    if (scaleValue == 0)
      delta_scale = 0.1f;
    else
      delta_scale = scaleValue;
    if (scaleF == scalePlus) {
      affine_transform.scaleFactor += delta_scale;
    } else {
      affine_transform.scaleFactor -= delta_scale;
    }
    // Ограничение масштаба
    if (affine_transform.scaleFactor < 0.1f)
      affine_transform.scaleFactor = 0.1f;
    if (affine_transform.scaleFactor > 10.0f)
      affine_transform.scaleFactor = 10.0f;
  }
}

/**
 * @brief Сброс параметров фигуры к значениям по умолчанию
 *
 * @param flag Флаг, указывающий, нужно ли сбрасывать дополнительные параметры
 */
void ViewerModel::setDefault(int flag) {
  affine_transform.projectionType = Parallel;
  affine_transform.rotateAngleX = 0.0f;
  affine_transform.rotateAngleY = 0.0f;
  affine_transform.rotateAngleZ = 0.0f;
  affine_transform.scaleFactor = 1.0f;
  affine_transform.translateX = 0.0f;
  affine_transform.translateY = 0.0f;
  affine_transform.translateZ = 0.0f;
  affine_transform.fov = 45.0f;
  if (flag) {
    modelDefinition.facetWidth = 0.0f;
    modelDefinition.verticeType = Square;
    modelDefinition.verticeWidth = 5.0f;
  }
}

/**
 * @brief Установка ширины граней фигуры
 *
 * @param scaleF Тип изменения ширины (scalePlus или scaleMinus)
 */
void ViewerModel::setFacetWidth(const ScaleType_t &scaleF) {
  if (scaleF == scalePlus) {
    if (modelDefinition.facetWidth == 0) {
      modelDefinition.facetWidth = 0.005;
    } else {
      modelDefinition.facetWidth += 0.0005;
    }
  } else if (scaleF == scaleMinus) {
    if (modelDefinition.facetWidth == 0) {
      modelDefinition.facetWidth = 0.005;
    } else if (modelDefinition.facetWidth >= 0.001) {
      modelDefinition.facetWidth -= 0.0005;
    }
  }
}

/**
 * @brief Установка цвета граней фигуры
 *
 * @param facetColor Цвет граней
 * @param modelDefinition Структура, содержащая параметры модели
 */
void SetFacetColor::setColor(const QColor &facetColor,
                             ModelDefinition_t &modelDefinition) {
  modelDefinition.facetColor = facetColor;
}

/**
 * @brief Установка цвета вершин фигуры
 *
 * @param verticeColor Цвет вершин
 * @param modelDefinition Структура, содержащая параметры модели
 */
void SetVerticeColor::setColor(const QColor &verticeColor,
                               ModelDefinition_t &modelDefinition) {
  modelDefinition.verticeColor = verticeColor;
}

/**
 * @brief Установка цвета фона
 *
 * @param backGroundColor Цвет фона
 * @param modelDefinition Структура, содержащая параметры модели
 */
void SetBackGroundColor::setColor(const QColor &backGroundColor,
                                  ModelDefinition_t &modelDefinition) {
  modelDefinition.backgroundColor = backGroundColor;
}

/**
 * @brief Конструктор класса ViewerModel
 */
ViewerModel::ViewerModel() : setColor_(nullptr) { readModelDefinition(); };

/**
 * @brief Деструктор класса ViewerModel
 */
ViewerModel::~ViewerModel() {
  saveModelDefinition();
  delete setColor_;
};

/**
 * @brief Установка стратегии изменения цвета
 *
 * @param strategy Стратегия изменения цвета
 * @param color Цвет, который нужно установить
 */
void ViewerModel::SetColorStrategy(SetColor *strategy, const QColor &color) {
  if (setColor_) {
    delete setColor_;
  }
  setColor_ = strategy;
  setColor_->setColor(color, modelDefinition);
}

/**
 * @brief Установка цвета граней фигуры
 *
 * @param FacetColor Цвет граней
 */
void ViewerModel::setFacetColor(const QColor &FacetColor) {
  SetFacetColor *setFacet = new SetFacetColor();
  SetColorStrategy(setFacet, FacetColor);
}

/**
 * @brief Установка цвета вершин фигуры
 *
 * @param VerticeColor Цвет вершин
 */
void ViewerModel::setVerticeColor(const QColor &VerticeColor) {
  SetVerticeColor *setVertice = new SetVerticeColor();
  SetColorStrategy(setVertice, VerticeColor);
}

/**
 * @brief Установка цвета фона
 *
 * @param backGroundColor Цвет фона
 */
void ViewerModel::setBackGroundColor(const QColor &backGroundColor) {
  SetBackGroundColor *setBackGround = new SetBackGroundColor();
  SetColorStrategy(setBackGround, backGroundColor);
}

/**
 * @brief Установка ширины вершин фигуры
 *
 * @param scaleF Тип изменения ширины (scalePlus или scaleMinus)
 */
void ViewerModel::setVerticeWidth(const ScaleType_t &scaleF) {
  if (scaleF == scalePlus) {
    if (modelDefinition.verticeWidth <= 12.0f)
      modelDefinition.verticeWidth += 1.0f;
  } else if (scaleF == scaleMinus) {
    if (modelDefinition.verticeWidth >= 2.0f)
      modelDefinition.verticeWidth -= 1.0f;
  }
}

/**
 * @brief Установка типа вершин фигуры
 *
 * @param verticeType Тип вершин (Square, Circle или None)
 */
void ViewerModel::setVerticeType(const VerticeType_t &verticeType) {
  modelDefinition.verticeType = verticeType;
}

/**
 * @brief Обработка перемещения мыши (вращение фигуры)
 *
 * @param delta Изменение координат мыши
 */
void ViewerModel::MouseButtonMove(QPoint delta) {
  affine_transform.rotateAngleX += delta.y();  // вращение вокруг оси X
  affine_transform.rotateAngleY += delta.x();  // вращение вокруг оси Y
}

/**
 * @brief Обработка прокрутки колеса мыши (вращение фигуры вокруг оси Z)
 *
 * @param delta Изменение координат колеса мыши
 */
void ViewerModel::MouseWheelMove(QPoint delta) {
  affine_transform.rotateAngleZ += delta.x();  // вращение вокруг оси Z
}

/**
 * @brief Загрузка модели из файла OBJ
 *
 * @param filePath Путь к файлу OBJ
 */
void ViewerModel::loadOBJ(const QString &filePath) {
  ViewerModel::setDefault(0);
  affine_transform.rotateAngleX = 0.0f;
  affine_transform.rotateAngleY = 0.0f;
  affine_transform.rotateAngleZ = 0.0f;
  affine_transform.scaleFactor = 1.0f;
  affine_transform.translateX = 0.0f;
  affine_transform.projectionType = Parallel;
  vertices.clear();
  facets.clear();

  std::string path = filePath.toStdString();
  std::ifstream file(path);
  if (!file) {
    qWarning() << "Failed to open file:" << path.c_str();
    return;
  }

  std::string line;
  while (std::getline(file, line)) {
    if (line.substr(0, 2) == "v ") {
      std::istringstream s(line.substr(2));
      QVector3D v;
      s >> v[0] >> v[1] >> v[2];
      vertices.push_back(v);
    } else if (line.substr(0, 2) == "f ") {
      std::istringstream s(line.substr(2));
      std::string token;
      while (s >> token) {
        unsigned int index = std::stoul(token.substr(0, token.find('/'))) - 1;
        facets.push_back(index);
      }
    }
  }
  file.close();
  normalizeVertices();
}

/**
 * @brief Получение вершин модели
 *
 * @return Вектор вершин модели
 */
std::vector<QVector3D> ViewerModel::getVertices() { return vertices; };

/**
 * @brief Получение ребер модели
 *
 * @return Вектор ребер модели
 */
std::vector<unsigned int> ViewerModel::getFacets() { return facets; };

/**
 * @brief Получение параметров аффинных преобразований
 *
 * @return Структура, содержащая параметры аффинных преобразований
 */
AffineTransform_t ViewerModel::getAffineTransform() {
  return affine_transform;
};

/**
 * @brief Получение параметров модели
 *
 * @return Структура, содержащая параметры модели
 */
ModelDefinition_t ViewerModel::getModelDefinition() { return modelDefinition; }

/**
 * @brief Нормализация вершин модели (центрирование и масштабирование)
 */
void ViewerModel::normalizeVertices() {
  if (vertices.empty()) return;

  // Находим минимальные и максимальные значения по всем осям
  float minX = vertices[0].x(), maxX = vertices[0].x();
  float minY = vertices[0].y(), maxY = vertices[0].y();
  float minZ = vertices[0].z(), maxZ = vertices[0].z();

  for (const auto &v : vertices) {
    minX = std::min(minX, v.x());
    maxX = std::max(maxX, v.x());
    minY = std::min(minY, v.y());
    maxY = std::max(maxY, v.y());
    minZ = std::min(minZ, v.z());
    maxZ = std::max(maxZ, v.z());
  }

  // Находим центр модели
  float centerX = (minX + maxX) / 2.0f;
  float centerY = (minY + maxY) / 2.0f;
  float centerZ = (minZ + maxZ) / 2.0f;

  // Находим максимальный размер модели по всем осям
  float sizeX = maxX - minX;
  float sizeY = maxY - minY;
  float sizeZ = maxZ - minZ;
  float maxSize = std::max({sizeX, sizeY, sizeZ});

  // Нормализация: перемещаем модель в центр и масштабируем
  for (auto &v : vertices) {
    v.setX((v.x() - centerX) / maxSize * 2.0f);  // Нормализация по X
    v.setY((v.y() - centerY) / maxSize * 2.0f);  // Нормализация по Y
    v.setZ((v.z() - centerZ) / maxSize * 2.0f);  // Нормализация по Z
  }
}

/**
 * @brief Сохранение параметров модели в файл
 */
void ViewerModel::saveModelDefinition() {
  QFile file("../model.txt");
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
    qWarning() << "File not found";
    return;
  }
  QTextStream out(&file);
  out << modelDefinition.facetColor.name() << " "
      << modelDefinition.verticeColor.name() << " "
      << modelDefinition.backgroundColor.name() << " "
      << modelDefinition.facetWidth << " " << modelDefinition.verticeType << " "
      << modelDefinition.verticeWidth;
  file.close();
}

/**
 * @brief Чтение параметров модели из файла
 */
void ViewerModel::readModelDefinition() {
  QFile file("../model.txt");
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    qWarning() << "File not found";
    return;
  }
  QTextStream in(&file);
  QString line = in.readLine().trimmed();
  file.close();
  QStringList definition = line.split(" ");
  if (definition.size() != 6) {
    qWarning() << "Файл не содержит корректных значений:" << line;
  }
  QString facetColorHex = definition[0];
  QString verticeColorHex = definition[1];
  QString backgroundColorHex = definition[2];
  modelDefinition.facetColor = QColor(facetColorHex);
  modelDefinition.verticeColor = QColor(verticeColorHex);
  modelDefinition.backgroundColor = QColor(backgroundColorHex);

  float facetWidth = definition[3].toFloat();
  VerticeType_t verticeType;
  if (definition[4] == "0")
    verticeType = Square;
  else if (definition[4] == "1")
    verticeType = Circle;
  else if (definition[4] == "2")
    verticeType = None;
  float verticeWidth = definition[5].toFloat();

  modelDefinition.facetWidth = facetWidth;
  modelDefinition.verticeType = verticeType;
  modelDefinition.verticeWidth = verticeWidth;
}

/**
 * @brief Преобразование строки в число с плавающей точкой
 *
 * @param inputText Входная строка
 * @return Число с плавающей точкой
 */
float ViewerModel::makeFloat(const QString &inputText) {
  QRegularExpression regex("[^0-9.]");
  QString filteredText = inputText;
  filteredText.remove(regex);
  float translateValue = filteredText.toFloat();
  return translateValue;
}

}  // namespace s21