import pandas as pd
import numpy as np
from sklearn.preprocessing import LabelEncoder, StandardScaler
from sklearn.svm import SVC
from sklearn.model_selection import GridSearchCV
from sklearn.metrics import f1_score, make_scorer
from sklearn.utils.class_weight import compute_class_weight

# Load the data
train_data = pd.read_csv('train.csv')
test_data = pd.read_csv('test.csv')

# Define mappings for features that need to be labeled
party_labels = {party: idx for idx, party in enumerate(train_data['Party'].unique())}
state_labels = {state: idx for idx, state in enumerate(train_data['state'].unique())}

# Map the labels to the DataFrame
train_data['Party'] = train_data['Party'].map(party_labels)
train_data['state'] = train_data['state'].map(state_labels)
test_data['Party'] = test_data['Party'].map(party_labels)
test_data['state'] = test_data['state'].map(state_labels)

# Convert 'Total Assets' and 'Liabilities' columns to integers
train_data['Total Assets'] = train_data['Total Assets'].str.replace(r' Crore\+', '0000000', regex=True).str.replace(r' Lac\+', '00000', regex=True).str.replace(r' Thou\+', '000', regex=True).str.replace(r' Hund\+', '00', regex=True).astype(int)
train_data['Liabilities'] = train_data['Liabilities'].str.replace(r' Crore\+', '0000000', regex=True).str.replace(r' Lac\+', '00000', regex=True).str.replace(r' Thou\+', '000', regex=True).str.replace(r' Hund\+', '00', regex=True).astype(int)
test_data['Total Assets'] = test_data['Total Assets'].str.replace(r' Crore\+', '0000000', regex=True).str.replace(r' Lac\+', '00000', regex=True).str.replace(r' Thou\+', '000', regex=True).str.replace(r' Hund\+', '00', regex=True).astype(int)
test_data['Liabilities'] = test_data['Liabilities'].str.replace(r' Crore\+', '0000000', regex=True).str.replace(r' Lac\+', '00000', regex=True).str.replace(r' Thou\+', '000', regex=True).str.replace(r' Hund\+', '00', regex=True).astype(int)

# Encode categorical variables
label_encoder = LabelEncoder()
train_data['Education'] = label_encoder.fit_transform(train_data['Education'])

# Drop unnecessary columns
X_train = train_data.drop(['ID', 'Candidate', 'Constituency ∇', 'Education'], axis=1)
y_train = train_data['Education']
X_test = test_data.drop(['ID', 'Candidate', 'Constituency ∇'], axis=1)

# Standardize the features
scaler = StandardScaler()
X_train_scaled = scaler.fit_transform(X_train)
X_test_scaled = scaler.transform(X_test)

# Compute class weights
class_weights = compute_class_weight(class_weight='balanced', classes=np.unique(y_train), y=y_train)
class_weights_dict = dict(enumerate(class_weights))

# Define the SVM classifier with class weights
svm = SVC(class_weight=class_weights_dict)

# Define hyperparameters
param_grid = {
    'C': [0.1, 1, 10, 100, 1000],
    'kernel': ['rbf','linear'],
    'degree': [1,2]
}

# Create a scorer for the F1 score
f1_scorer = make_scorer(f1_score, average='weighted')

# Perform grid search cross-validation to find the best hyperparameters
grid_search = GridSearchCV(estimator=svm, param_grid=param_grid, cv=5, scoring=f1_scorer, verbose=2)
grid_search.fit(X_train_scaled, y_train)

# Print the best hyperparameters found
print("Best Hyperparameters:", grid_search.best_params_)
print("Best F1 Score:", grid_search.best_score_)

# Fit the model with the best hyperparameters
best_svm = grid_search.best_estimator_
best_svm.fit(X_train_scaled, y_train)

# Predictions on test set
y_pred_test = best_svm.predict(X_test_scaled)

# Inverse transform the encoded predictions
inverse_predicted_education = label_encoder.inverse_transform(y_pred_test)

# Create submission dataframe
submission_df = pd.DataFrame({'ID': test_data['ID'], 'Education': inverse_predicted_education})

# Save submission to CSV
submission_df.to_csv('svc.csv', index=False)