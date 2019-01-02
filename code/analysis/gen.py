import pandas
INPUT = 'stat1'
OUTPUT = 'process.csv'
r = pandas.read_csv(INPUT)
r = r[r[' newrecord'] == 1]
r.to_csv(OUTPUT)