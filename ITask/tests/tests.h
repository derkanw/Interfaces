#pragma once
#include <array>
#include <memory>
#include <iostream>
#include <cmath>
#include "../include/IVector.h"
#include "../include/ILogger.h"
#include "../include/ISet.h"
#include "../include/IMultiIndex.h"
#include "../include/ICompact.h"

#define TOL 1e-3
#define DEFAULT_NORM IVector::NORM::FIRST
#define DIM 3

#define LOGGER ILogger* logger = ILogger::createLogger("log.txt"); IVector::setLogger(logger); ISet::setLogger(logger); \
    IMultiIndex::setLogger(logger); ICompact::setLogger(logger);
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

#define INDEX1 size_t arr1[] = {10, 5, 4}; IMultiIndex *index1 = IMultiIndex::createMultiIndex(DIM, arr1);
#define INDEX2 size_t arr2[] = {1, 2, 3}; IMultiIndex *index2 = IMultiIndex::createMultiIndex(DIM, arr2);
#define DELETE_INDEX1 delete index1;
#define DELETE_INDEX2 delete index2;

#define COMPACT1 double arr1[] = {0, 0}; IVector* left1 = IVector::createVector(sizeof(arr1), arr1); \
    double arr2[] = {4, 4}; IVector* right1 = IVector::createVector(sizeof(arr2), arr2); \
    size_t arr3[] = {5, 5}; IMultiIndex *grid1 = IMultiIndex::createMultiIndex(sizeof(arr3), arr3); \
    ICompact *compact1 = ICompact::createCompact(left1, right1, grid1);
#define COMPACT2 double arr4[] = {2, 2}; IVector* left2 = IVector::createVector(sizeof(arr4), arr4); \
    double arr5[] = {7, 7}; IVector* right2 = IVector::createVector(sizeof(arr5), arr5); \
    size_t arr6[] = {5, 5}; IMultiIndex *grid2 = IMultiIndex::createMultiIndex(sizeof(arr6), arr6); \
    ICompact *compact2 = ICompact::createCompact(left2, right2, grid2);
#define DELETE_COMPACT1 delete left1; delete right1; delete grid1; delete compact1;
#define DELETE_COMPACT2 delete left2; delete right2; delete grid2; delete compact2;                                                                                                \
