#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri Dec  6 11:42:35 2019

@author: julieosborne
"""
import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.linear_model import LogisticRegression

full_data = pd.read_csv('/Users/julieosborne/Documents/machine learning/crime.csv', encoding="ISO-8859-1", low_memory=False)
print(full_data.head())
data1 = full_data[['DISTRICT','OFFENSE_CODE_GROUP', 'DAY_OF_WEEK', 'HOUR', 'MONTH', 'YEAR' ]]

day_month=data1['MONTH'].astype(str) + '/' + data1['YEAR'].astype(str)
d = {'MONTH_YEAR' : day_month}
df = pd.DataFrame(data=d)
new=pd.concat([data1, df], axis=1)

data=new[['DISTRICT','OFFENSE_CODE_GROUP', 'DAY_OF_WEEK', 'HOUR', 'MONTH_YEAR']]


##One hot encoding all the categorical variables 
cat_data_offense_onehot = data.copy()
cat_data_offense_onehot = pd.get_dummies(cat_data_offense_onehot, columns=['OFFENSE_CODE_GROUP'], prefix=['offense'])

cat_data_week_onehot= cat_data_offense_onehot.copy()
cat_data_week_onehot = pd.get_dummies(cat_data_week_onehot, columns=['DAY_OF_WEEK'], prefix=['week'])

cat_data_hour_onehot= cat_data_week_onehot.copy()
cat_data_hour_onehot = pd.get_dummies(cat_data_hour_onehot, columns=['HOUR'], prefix=['hour'])

cat_data_month_year_onehot= cat_data_hour_onehot.copy()
cat_data_month_year_onehot = pd.get_dummies(cat_data_month_year_onehot, columns=['MONTH_YEAR'], prefix=['month_year'])

print(cat_data_month_year_onehot.head())

data_new = cat_data_month_year_onehot
print(data_new.head())

#district = pd.get_dummies(data_new['DISTRICT'])
#data_new = pd.concat([data_new, district], axis=1)

##Split into training & test set 
data_new['DISTRICT'] = data_new['DISTRICT'].astype('category')
data_new['district_cat'] = data_new['DISTRICT'].cat.codes

data_new=data_new.dropna()
data_new['DISTRICT'] = data_new['DISTRICT'].astype('category')
data_new['district_cat'] = data_new['DISTRICT'].cat.codes
data_new.district_cat.unique()



d = data_new['DISTRICT'].astype('category')
dict1 = dict(enumerate(d.cat.categories))
print(dict1)

data_new1=data_new.copy()
del data_new1['DISTRICT']



#data_new['DISTRICT'].astype('category')
#y= data_new.DISTRICT
y=data_new1.district_cat
X = data_new1.drop(['district_cat'], axis=1)
X.head()
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=123)
print(X_train.shape, y_train.shape)
print(X_test.shape, y_test.shape)



model = LogisticRegression(solver = 'lbfgs')
model.fit(X_train, y_train)

y_pred=model.predict(X_test)

from sklearn import metrics
cnf_metric = metrics.confusion_matrix(y_test, y_pred)
cnf_metric

print('Accuracy:', metrics.accuracy_score(y_test, y_pred))


import matplotlib.pyplot as plt
import seaborn as sns

ax = plt.subplot()
sns.heatmap(cnf_metric, annot=True, ax=ax, fmt='g', cmap="Blues")
ax.set_xlabel('Predicted labels'); ax.set_ylabel('True labels');
ax.xaxis.set_ticklabels(['A1', 'A15', 'A7', 'B2','B3','C11','C6','D14','D4','E13','E18','E5'])
ax.yaxis.set_ticklabels(['A1', 'A15', 'A7', 'B2','B3','C11','C6','D14','D4','E13','E18','E5']);
ax.set_title('Accuracy Logistic Regression: '+str(metrics.accuracy_score(y_test, y_pred)));




######Trying PCA to see if it improves accuracy 

from sklearn.decomposition import PCA

pca = PCA(0.95)
pca.fit(X_train)
n_components = pca.n_components
train_data = pca.transform(X_train)

test_data = pca.transform(X_test)
logisticReg = LogisticRegression(solver='lbfgs')
logisticReg.fit(test_data, y_test)

#predict
pred1 = logisticReg.predict(test_data)

cnf_metric = metrics.confusion_matrix(y_test, pred1)
cnf_metric

print('Accuracy:', metrics.accuracy_score(y_test, pred1))



####Decision Tree
from sklearn.tree import DecisionTreeClassifier

    
clf = DecisionTreeClassifier(criterion="entropy", max_depth=26, random_state=123)

# Train Decision Tree Classifer
clf = clf.fit(X_train,y_train)

#Predict the response for test dataset
y_pred1 = clf.predict(X_test)


print('Accuracy: ',metrics.accuracy_score(y_test, y_pred1))

ax1 = plt.subplot()
sns.heatmap(cnf_metric, annot=True, ax=ax1, fmt='g', cmap='Greens')
ax1.set_xlabel('Predicted labels'); ax1.set_ylabel('True labels');
ax1.xaxis.set_ticklabels(['A1', 'A15', 'A7', 'B2','B3','C11','C6','D14','D4','E13','E18','E5'])
ax1.yaxis.set_ticklabels(['A1', 'A15', 'A7', 'B2','B3','C11','C6','D14','D4','E13','E18','E5']);
ax1.set_title('Accuracy Decision Tree: '+str(metrics.accuracy_score(y_test, y_pred1)));




