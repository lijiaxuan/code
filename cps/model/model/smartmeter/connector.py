__author__ = 'sapphire'
from pymongo import MongoClient,collection,database
con=MongoClient("localhost",27017)
db=database.Database(con,'mydb')
def insert_bus_data(data):
    db.att.insert_one(data)
def update_bus_data(no,data):
    db.att.replace_one({'bus_id':no},data,True)

# for i in range(39):
#   update_bus_data(i+1,{'bus_id':i+1,'attack':1})
#db.att.remove()
#for i in range(39):
#   insert_bus_data({'bus_id':i+1,'attack':0})
# for i in db.att.find():
#      print i
