#include "simple_feature_value_subject.h"
#include "simple_feature_value_observer.h"

SimpleFeatureValueSubject::SimpleFeatureValueSubject()
{
   simpleFeatureValueObservers = new QVector<SimpleFeatureValueObserver*>();
}




void SimpleFeatureValueSubject::addSimpleFeatureValueObserver(SimpleFeatureValueObserver* observer)
{
    // todo: add check that not already added
    simpleFeatureValueObservers->append(observer);

}




void SimpleFeatureValueSubject::simpleFeatureValueNotify(SimpleFeatureValue fv)
{
    for (int ndx = 0; ndx < simpleFeatureValueObservers->count(); ndx++) {
        printf("(%s) Notifying observer\n", __func__);  fflush(stdout);
        SimpleFeatureValueObserver * obs = simpleFeatureValueObservers->at(ndx);
        obs->simpleFeatureValueChanged(fv);
    }

}


