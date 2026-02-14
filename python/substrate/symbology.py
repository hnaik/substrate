import databento as db

client = db.Historical('YOUR_API_KEY')

# Convert ticker (raw_symbol) to instrument_id
result = client.symbology.resolve(
    dataset='GLBX.MDP3',
    symbols=['ESM2'],
    stype_in='raw_symbol',
    stype_out='instrument_id',
    start_date='2022-06-01',
    end_date='2022-06-30',
)

# Result shows: ESM2 → 3403
instrument_id = result['ESM2']['instrument_id']
print(f'Instrument ID for ESM2: {instrument_id}')  # Should print:
