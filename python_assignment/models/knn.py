import pandas as pd
import numpy as np
from sklearn.preprocessing import LabelEncoder, StandardScaler
from sklearn.neighbors import KNeighborsClassifier
from sklearn.metrics import f1_score

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

# Modifying Total Assets and Liabilities so that they are treated as numbers
train_data['Total Assets'] = train_data['Total Assets'].str.replace(r' Crore\+', '00000', regex=True).str.replace(r' Lac\+', '000', regex=True).str.replace(r' Thou\+', '0', regex=True).str.replace(r' Hund\+', '', regex=True).astype(int)
train_data['Liabilities'] = train_data['Liabilities'].str.replace(r' Crore\+', '00000', regex=True).str.replace(r' Lac\+', '000', regex=True).str.replace(r' Thou\+', '0', regex=True).str.replace(r' Hund\+', '', regex=True).astype(int)
test_data['Total Assets'] = test_data['Total Assets'].str.replace(r' Crore\+', '00000', regex=True).str.replace(r' Lac\+', '000', regex=True).str.replace(r' Thou\+', '0', regex=True).str.replace(r' Hund\+', '', regex=True).astype(int)
test_data['Liabilities'] = test_data['Liabilities'].str.replace(r' Crore\+', '00000', regex=True).str.replace(r' Lac\+', '000', regex=True).str.replace(r' Thou\+', '0', regex=True).str.replace(r' Hund\+', '', regex=True).astype(int)

# Encode categorical variables
label_encoder = LabelEncoder()
train_data['Education'] = label_encoder.fit_transform(train_data['Education'])

# Drop unnecessary columns
X_train = train_data.drop(['ID', 'Candidate', 'Constituency ∇', 'Total Assets', 'Liabilities', 'Education'], axis=1)
y_train = train_data['Education']
X_test = test_data.drop(['ID', 'Candidate', 'Constituency ∇', 'Total Assets', 'Liabilities'], axis=1)

# Standardize the features
scaler = StandardScaler()
X_train_scaled = scaler.fit_transform(X_train)
X_test_scaled = scaler.transform(X_test)

# Define the kNN classifier
knn = KNeighborsClassifier()

# Define hyperparameters to tune
param_grid = {
    'n_neighbors': [20, 40, 60, 80, 100],
    'weights': ['uniform'],
    'algorithm': ['brute','auto'],
    'p': [1]
}

best_f1_score = 0
best_params = None
best_model = None

# Iterate over different random seeds
for seed in range(0, 10):
    print(f"Random Seed: {seed}")
    
    # Set random seed for reproducibility
    np.random.seed(seed)
    
    # Randomly sample hyperparameters from the parameter grid
    random_params = {k: np.random.choice(v) for k, v in param_grid.items()}
    print("Randomly Sampled Hyperparameters:", random_params)
    
    # Create a kNN classifier with the randomly sampled hyperparameters
    random_knn = KNeighborsClassifier(**random_params)
    
    # Fit the classifier on the training data
    random_knn.fit(X_train_scaled, y_train)
    
    # Predict on the training data
    y_pred_train = random_knn.predict(X_train_scaled)
    
    # Calculate the F1 score 
    f1 = f1_score(y_train, y_pred_train, average='macro')
    
    # Update if the current F1 score is higher
    if f1 > best_f1_score:
        best_f1_score = f1
        best_params = random_params
        best_model = random_knn

print("Best Hyperparameters:", best_params)
print("Best Training F1 Score:", best_f1_score)

# Predict on the test data
y_pred_test = best_model.predict(X_test_scaled)

# Inverse transform
inverse_predicted_education = label_encoder.inverse_transform(y_pred_test)

# Create submission dataframe and save to csv
submission_df = pd.DataFrame({'ID': test_data['ID'], 'Education': inverse_predicted_education})
submission_df.to_csv('knn.csv', index=False)