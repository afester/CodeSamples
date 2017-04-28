use sampleDatabase

db.myCollection.insert( { name : "LED, 5mm, red", color : "red", diameter : 0.005 } );
db.myCollection.insert( { name : "LED, 5mm, green", color : "green", diameter : 0.005 } );
db.myCollection.insert( { name : "LED, 3mm, red", color : "red", diameter : 0.003 } );
db.myCollection.insert( { name : "Lego brick 4x4", color : "red", width : 4, length : 4 } );

db.myCollection.find({diameter: 0.005});
db.myCollection.find({color:"red"});
