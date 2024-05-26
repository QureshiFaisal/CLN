#!/usr/bin/env python3

from pyln.client import Plugin
import requests

plugin = Plugin()

@plugin.method("satoshis_to_inr")
def convert_satoshis_to_inr(plugin, satoshis):
    # Fetch Bitcoin to INR exchange rate from CoinGecko API
    try:
        response = requests.get("https://api.coingecko.com/api/v3/simple/price?ids=bitcoin&vs_currencies=inr")
        if response.status_code == 200:
            bitcoin_inr_rate = response.json()["bitcoin"]["inr"]
            # Convert satoshis to Bitcoin
            bitcoin_amount = satoshis / 10**8
            # Convert Bitcoin to INR
            inr_amount = bitcoin_amount * bitcoin_inr_rate
            return f"Your invoice is of {satoshis} satoshis which is approximately {inr_amount:.2f} INR"
        else:
            return "Failed to fetch exchange rate from CoinGecko API"
    except Exception as e:
        plugin.log(f"Error fetching exchange rate: {e}")
        return "Failed to fetch exchange rate"

@plugin.init()
def init(options, configuration, plugin):
    plugin.log("Satoshis to INR Plugin initialized!")

plugin.run()