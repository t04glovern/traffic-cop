var leafletMap = L.map('mapid').setView([-31.99388, 115.9108], 13);

L.tileLayer('https://api.tiles.mapbox.com/v4/{id}/{z}/{x}/{y}.png?access_token={accessToken}', {
    attribution: 'Map data &copy; <a href="https://www.openstreetmap.org/">OpenStreetMap</a> contributors, <a href="https://creativecommons.org/licenses/by-sa/2.0/">CC-BY-SA</a>, Imagery © <a href="https://www.mapbox.com/">Mapbox</a>',
    maxZoom: 18,
    id: 'mapbox.streets',
    accessToken: 'pk.eyJ1IjoidDA0Z2xvdmVybiIsImEiOiJjam4xa2pkcGk0bGh3M3FvZXhiZmlycWdrIn0._-6n6Fe0A8YJS8pQQ-sJLw'
}).addTo(leafletMap);
