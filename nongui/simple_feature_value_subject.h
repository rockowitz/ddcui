// defines a mixin

#ifndef SIMPLEFEATUREVALUESUBJECT_H
#define SIMPLEFEATUREVALUESUBJECT_H

#include <QVector>

#include "simple_feature_value.h"
#include "simple_feature_value_observer.h"

class SimpleFeatureValueSubject
{
public:
    SimpleFeatureValueSubject();


    void addSimpleFeatureValueObserver(SimpleFeatureValueObserver* observer);

protected:
    void simpleFeatureValueNotify(SimpleFeatureValue fv);

private:
    QVector<SimpleFeatureValueObserver*> * simpleFeatureValueObservers;

};

#endif // SIMPLEFEATUREVALUESUBJECT_H
