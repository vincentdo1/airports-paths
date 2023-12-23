import pandas as pd
import numpy as np
import requests
from bs4 import BeautifulSoup
#edges (ID node1 node2)
edges = pd.read_csv("routes.csv", header=None)
edges.drop(edges.columns[[0,1,3,5,6,7,8]], axis=1, inplace=True)
edges.drop_duplicates(inplace=True)
edges.rename(columns={2: "From", 4:"To"}, inplace=True)
edges["ID"] = edges.apply(lambda row: row["From"]+row["To"], axis=1)
edges = edges[["ID", "From", "To"]]
np.savetxt("origEdges.txt", edges.to_numpy(), fmt = "%s")
#Nodes
nodes = pd.read_csv("airports.csv", header=None)
nodes.drop(nodes.columns[[0,1,2,3,5,8,9,10,11,12,13]], axis=1, inplace=True)
nodes.drop(nodes.index[nodes.iloc[:,0] == "\\N"], inplace=True)
nodes.rename(columns={4:"ID", 6:"Latitude", 7: "Longitude"}, inplace=True)
np.savetxt("origNodes.txt", nodes.to_numpy(), fmt = "%s")
#Top 500 Nodes
site = 'https://gettocenter.com/airports/top-100-airports-in-world/1000'
page = requests.get(site)
soup = BeautifulSoup(page.text, 'html.parser')
table1 = soup.find("table", attrs={"class": "table"})
table2 = soup.find_all('tr')
airports = []
for row in table2:
    airports.append(row.find_all("td")[2].text.strip())
nodes_500 = nodes[nodes.ID.isin(airports)]
np.savetxt("nodes1000.txt", nodes_500.to_numpy(), fmt = "%s")
#Top edges of 500 nodes
edges_500 = edges[edges.From.isin(airports)]
edges_500 = edges[edges.To.isin(airports)]
np.savetxt("edges1000.txt", edges_500.to_numpy(), fmt = "%s")
