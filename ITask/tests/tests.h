#pragma once
#include <array>
#include <memory>
#include <iostream>
#include <cmath>
#include "../include/IVector.h"
#include "../include/ILogger.h"
#include "../include/ISet.h"

#define TOL 1e-3
#define DEFAULT_NORM IVector::NORM::FIRST
#define DIM 3

#define LOGGER ILogger* logger = ILogger::createLogger("log.txt"); IVector::setLogger(logger); ISet::setLogger(logger);
#define VECTOR1 double arr1[] = { 1.1, 2.3, 3.5 }; IVector* vector1 = IVector::createVector(DIM, arr1);
#define VECTOR2 double arr2[] = { 5.3, 2.4, 1.9 }; IVector* vector2 = IVector::createVector(DIM, arr2);
#define VECTOR3 double arr3[] = { 3.1, 4.2, 6.5 }; IVector* vector3 = IVector::createVector(DIM, arr3);

#define DELETE_VECTOR1 delete vector1;
#define DELETE_VECTOR2 delete vector2;
#define DELETE_VECTOR3 delete vector3;
#define DELETE_LOGGER delete logger;

#define SET1 ISet* set1 = ISet::createSet();
#define SET2 ISet* set2 = ISet::createSet();
#define SET3 ISet* set3 = ISet::createSet();
#define DELETE_SET1 delete set1;
#define DELETE_SET2 delete set2;
#define DELETE_SET3 delete set3;
