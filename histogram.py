import pandas as pd
import matplotlib.pyplot as plt

# Load the CSV data (assumes CSV has a "missing_mass" column)
df = pd.read_csv('missing_mass2.csv')

# Plot the histogram
plt.figure(figsize=(8,6))
plt.hist(df['missing_mass'], bins=150, color='skyblue', edgecolor='black')
plt.xlabel('Missing Mass (GeV/c²)')
plt.ylabel('Frequency')
plt.title('Histogram of Missing Mass')
plt.tight_layout()
plt.savefig('missing_mass_histogram2.png')  # Save figure
plt.show()

