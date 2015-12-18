__author__ = 'sapphire'
from pymongo import MongoClient,collection,database
con=MongoClient("localhost",27017)
db=database.Database(con,'mydb')
def insert_bus_data(data):
    db.sensor.insert_one(data)
def update_bus_data(no,data):
    db.sensor.replace_one({'bus_id':no},data,True)
# update_bus_data(1,9)
#db.sensor.remove()
for i in db.sensor.find():
    print i