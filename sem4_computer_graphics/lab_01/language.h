#ifndef LANGUAGE_H
#define LANGUAGE_H

#define DEFAULT_STRING_START "> "

#define ENG 0
#define RUS 1

#define APP_LANGUAGE RUS

#if (APP_LANGUAGE == ENG)

// UI PART
#define HEADER_POINT_LABEL              "New Point"
#define HEADER_TABLE_LABEL              "Points Table"
#define HEADER_TRIANGLE_LABEL           "Triangle"
#define BUTTON_ADD_POINT                "Add Point"
#define BUTTON_DELETE_CURRENT_POINT     "Delete current point"
#define BUTTON_DELETE_ALL_POINTS        "Delete all points"
#define BUTTON_COMPUTE                  "Compute"

// ERRORS PART
#define ERR_MSG_START                   DEFAULT_STRING_START"Произошла ошибка. "

#define INVALID_POINT_EMPTY_FIELD       ERR_MSG_START"Одно из полей координат добавляемой точки пустое."
#define INVALID_POINT_VALIDATOR_ERR     ERR_MSG_START"Одна из составляющих координаты добавляемой точки (x или y) "\
"не может быть переведена в вещественный тип."


#define INVAILID_NTH_POINT_START        ERR_MSG_START"Координаты "
#define INVAILID_NTH_POINT_END          "й точки не верны."


#define INVALID_TABLE_CLEAR             ERR_MSG_START"Таблица не может быть очищена, потому что уже пуста."

#define INVALID_TRIANGLE_EMPTY_FIELD    ERR_MSG_START"Одно из полей координат треугольника пустое."
#define INVALID_TRIANGLE_VALIDATOR_ERR  ERR_MSG_START"Одна из составляющих координаты добавляемой точки (x или y) "\
    "треугольника не может быть переведена в вещественный тип."
#define INVALID_TRIANGLE                ERR_MSG_START"На данных точках нельзя построить треугольник."

#define NOT_ENOUGH_POINTS_FOR_CIRCLE    ERR_MSG_START"Недостаточно точек, чтобы описать хотя бы одну окружность."
#define NO_NEEDED_POINTS_FOR_CIRCLE     ERR_MSG_START"Нет таких трех точек, через которые можно провести окружность."
#define NO_NEEDED_CIRCLES_FOUND         ERR_MSG_START"Не найдено ни одной окружности, построенной хотя бы на трех данных точках, "\
    "центр которой бы лежал на прямой, проходящей через сторону треугольника."

#elif (APP_LANGUAGE == RUS)

// UI Part
#define HEADER_POINT_LABEL                  "Новая точка"
#define HEADER_TABLE_LABEL                  "Таблица точек"
#define HEADER_TRIANGLE_LABEL               "Треугольник"
#define BUTTON_ADD_POINT                    "Добавить точку"
#define BUTTON_DELETE_CURRENT_POINT         "Удалить текущую точку"
#define BUTTON_DELETE_ALL_POINTS            "Удалить все точки"
#define BUTTON_COMPUTE                      "Вычислить"
#define BUTTON_CLEAR_TRIANGLE               "Очистить точки треугольника"

// Errors Part
#define ERR_MSG_START                       DEFAULT_STRING_START"Произошла ошибка. "

#define INVALID_POINT_EMPTY_FIELD           ERR_MSG_START"Одно из полей координат добавляемой точки пустое."
#define INVALID_POINT_VALIDATOR_ERR         ERR_MSG_START"Одна из составляющих координаты добавляемой точки (x или y) "\
                                                         "не может быть переведена в вещественный тип."

#define INVALID_EDIT_EMPTY_FIELD            ERR_MSG_START"Одно из полей координат изменяемой точки пустое."
#define INVALID_EDIT_VALIDATOR_ERR          ERR_MSG_START"Одна из составляющих координаты изменяемой точки (x или y) "\
                                                         "не может быть переведена в вещественный тип."

#define INVAILID_NTH_POINT_EMPTY_START      ERR_MSG_START"Одно из полей координат "
#define INVAILID_NTH_POINT_EMPTY_END        "й точки пусто."

#define INVAILID_NTH_POINT_VALID_START      ERR_MSG_START"Одна из составляющих координаты "
#define INVAILID_NTH_POINT_VALID_END        "й точки (х или y) не может быть переведена в вещественный тип."

#define INVALID_TABLE_CLEAR                 ERR_MSG_START"Таблица не может быть очищена, потому что уже пуста."


#define INVALID_TRIANGLE_EMPTY_FIELD_START  ERR_MSG_START"Одно из полей координат точки "
#define INVALID_TRIANGLE_EMPTY_FIELD_END    " треугольника пустое."

#define INVALID_TRIANGLE_VAL_ERR_START      ERR_MSG_START"Одна из составляющих координаты точки "
#define INVALID_TRIANGLE_VAL_ERR_END        " (x или y) треугольника не может быть переведена в вещественный тип."

#define INVALID_TRIANGLE                    ERR_MSG_START"На данных точках нельзя построить треугольник."


#define NOT_ENOUGH_POINTS_FOR_CIRCLE        ERR_MSG_START"Недостаточно точек, чтобы описать хотя бы одну окружность."
#define NO_NEEDED_POINTS_FOR_CIRCLE         ERR_MSG_START"Нет таких трех точек, через которые можно провести окружность."
#define NO_NEEDED_CIRCLES_FOUND             ERR_MSG_START"Не найдено ни одной окружности, построенной хотя бы на трех данных точках, "\
                                                         "центр которой бы лежал на прямой, проходящей через сторону треугольника."

// Result Part
#define RESULT_START                        DEFAULT_STRING_START "Угол между осью Оу и прямой, проходящей через центр окружности, "\
                                            "построенной на точках "
#define RESULT_MIDDLE                       "и сторону заданного треугольника "
#define RESULT_END                          "° — наименьший."

#endif


#endif // LANGUAGE_H
