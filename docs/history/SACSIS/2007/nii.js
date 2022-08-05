var baseIcon = new GIcon();
baseIcon.shadow = "http://www.google.com/mapfiles/shadow50.png";
baseIcon.iconSize = new GSize(20, 34);
baseIcon.shadowSize = new GSize(37, 34);
baseIcon.iconAnchor = new GPoint(9, 34);
baseIcon.infoWindowAnchor = new GPoint(9, 2);
baseIcon.infoShadowAnchor = new GPoint(18, 25); 

function createMarker(point, letter, html)
{
	var icon = new GIcon(baseIcon);
	icon.image = "http://www.google.com/mapfiles/marker" + letter + ".png";
	var marker = new GMarker(point, icon);
	GEvent.addListener(marker, "click", function() {
		marker.openInfoWindowHtml(html);
	});
	return marker;
}

function Place (lat, lng, letter, html)
{
	this.html = html;
	this.point = new GLatLng(lat, lng);
	this.marker = new createMarker(this.point, letter, html);
}

function zoom(map, level, interval) {
	var zf = function() {
		map.zoomIn();
		if (map.getZoom() >= level)
			return;
		setTimeout(zf, interval);
	}
	setTimeout(zf, interval);
}

function load() {
  if (!GBrowserIsCompatible())
    return;

  var map = new GMap2(document.getElementById("map"));
  var tokyo = new Place(35.681056, 139.767079, "T", "Åìµþ±Ø");
  var nii = new Place(35.692581, 139.757887, "N", "³Ø½ÑÁí¹ç¥»¥ó¥¿¡¼");
  var take = new Place(35.690903, 139.756831, "T", "ÃÝ¶¶±Ø");
  var jin = new Place(35.693888, 139.758266, "J", "¿ÀÊÝÄ®±Ø");

  map.addControl(new GSmallMapControl());
  map.addControl(new GMapTypeControl());

  map.setCenter(tokyo.point, 15);
  map.addOverlay(nii.marker);
  nii.marker.openInfoWindowHtml(nii.html);

  while ( !map.isLoaded() );
  setTimeout(
    function () {
      map.panTo(nii.point);
      setTimeout(
        function () {
          map.setZoom(16);
	  map.addOverlay(take.marker);
	  map.addOverlay(jin.marker);
	  nii.marker.openInfoWindowHtml("³Ø½ÑÁí¹ç¥»¥ó¥¿¡¼<br>T: ¡ÖÃÝ¶¶±Ø¡×1b ½Ð¸ý<br>J: ¡Ö¿ÀÊÝÄ®±Ø¡×A8 ½Ð¸ý");
	  zoom(map, 18, 2500);
        },
        2500
      );
    },
    3000
  );


} 

