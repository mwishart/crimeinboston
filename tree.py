# It is defined by the kaggle/python docker image: https://github.com/kaggle/docker-python
# For example, here's several helpful packages to load in 

import numpy as np # linear algebra
import pandas as pd # data processing, CSV file I/O (e.g. pd.read_csv)

# Input data files are available in the "../input/" directory.
# For example, running this (by clicking run or pressing Shift+Enter) will list all files under the input directory

import os
for dirname, _, filenames in os.walk('/kaggle/input'):
    for filename in filenames:
        print(os.path.join(dirname, filename))

# Any results you write to the current directory are saved as output.
import pandas
#import csv 
input_file = '/kaggle/input/boston-crime-data/crime.csv'
df = pandas.read_csv(input_file, encoding = "ISO-8859-1")
df = df.drop(columns = ['INCIDENT_NUMBER', 'OFFENSE_CODE', 'OFFENSE_DESCRIPTION', 'REPORTING_AREA', 'SHOOTING', 
                  'UCR_PART', 'STREET', 'Lat', 'Long', 'Location', 'YEAR', 'MONTH','HOUR']) #drop unneeded columns
print(df)
#create training and testing sets 
train=df.sample(frac=0.75,random_state=200) #random state is a seed value
test=df.drop(train.index)