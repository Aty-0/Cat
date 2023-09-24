#pragma once
#include <iostream>
#include <map>
#include <array>
#include <vector>
#include <functional>
#include <memory>
#include <typeindex>
#include <fstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define VEC3_ONE  glm::vec3(1.0f, 1.0f, 1.0f)
#define VEC3_ZERO  glm::vec3(0.0f, 0.0f, 0.0f)
#define VEC2_ONE   glm::vec2(1.0f, 1.0f)
#define VEC2_ZERO  glm::vec2(0.0f, 0.0f)
#define VEC1_ONE   glm::vec1(1.0f)
#define VEC1_ZERO  glm::vec1(0.0f)

#include "core/engine_api.h"
#include "core/utils/utilsbase.h"
#include "core/utils/singleton.h"
