

import numpy as np
from sklearn.model_selection import train_test_split
from sklearn.feature_extraction.text import CountVectorizer
from sklearn.linear_model import LogisticRegression
from sklearn.metrics import accuracy_score

# Sample movie data (plot summaries and genres)
movie_data = [
    ("In a galaxy far, far away...", "Sci-Fi"),
    ("Once upon a time in a kingdom far, far away...", "Fantasy"),
    ("A group of friends embark on a journey to save the world.", "Adventure"),
    ("A young boy discovers he has magical powers.", "Fantasy"),
    ("An alien race threatens Earth's existence.", "Sci-Fi"),
    ("A detective solves a murder mystery.", "Mystery")
]

# Separate plot summaries and genres
plot_summaries, genres = zip(*movie_data)

# Convert genres to numerical labels
genre_to_label = {genre: i for i, genre in enumerate(set(genres))}
labels = np.array([genre_to_label[genre] for genre in genres])

# Split the data into training and testing sets
X_train, X_test, y_train, y_test = train_test_split(plot_summaries, labels, test_size=0.2, random_state=42)

# Vectorize plot summaries using CountVectorizer
vectorizer = CountVectorizer()
X_train_vectorized = vectorizer.fit_transform(X_train)
X_test_vectorized = vectorizer.transform(X_test)

# Train a logistic regression model
model = LogisticRegression(max_iter=1000)
model.fit(X_train_vectorized, y_train)

# Predict genre for test data
y_pred = model.predict(X_test_vectorized)

# Calculate accuracy
accuracy = accuracy_score(y_test, y_pred)
print("Accuracy:", accuracy)

