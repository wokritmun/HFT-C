import yfinance as yf
import os
print("Current Directory:", os.getcwd())

# Define the tickers list
tickers = ["AAPL", "MSFT", "GOOGL", "JPM", "BAC", "JNJ", "PFE", "PG", "KO", 
           "XOM", "CVX", "TSLA", "F", "GE", "T", "SPY", "QQQ", "MRNA", "GILD"]

# Directory to save the data
data_dir = "historical_data"
os.makedirs(data_dir, exist_ok=True)

# Fetch data
for ticker in tickers:
    print(f"Fetching data for {ticker}...")
    
    # Get the data for the stock/ETF
    data = yf.download(ticker, start="2000-01-01", end="2023-01-01")
    
    # Save to CSV
    data.to_csv(os.path.join(data_dir, f"{ticker}.csv"))

print("Data fetching completed!")