# CHANGELOG

## v0.9.0
- using std::shared_ptr<Product> now
- fixed issue #9, durability list is now sorted again.
- fixed issue #10, you can now add custom products again.

## v0.8.0
- some performance improvments and bug fixes
- calendar window is now bigger

## v0.7.0
- downloads automaticly the ean_codes.xml from github.

## v0.6.0
- veterinarian products can now be searched and added to the list.
- print is now supported, print the list based on durability.

## v0.5.0
- location of the "holdbarhetsliste" button is moved
- did some cleanup in the code
- more products can be found by using data matrix codes.

## v0.4.0
- Removed the save button, its not used anymore. You can now double click to update the location and date
- Date is passed to the widget when you updating the location or date.
- Whole line is now in color.
- New loadingscreen

## v0.3.0
- You can now scan 2d matrix codes (FMD) and it will automaticly add the product to the durability list with durability date.
- Double clicking the products will open a new window where you can change location and date.

## v0.2.0
- You can now add custom products to the database.
- compiler and qt version is added at compile time to about_apotekholdbarhet.
- the date is now in dd.MM.yyyy format.
- fixed a bug where you loaded files into the database twice. The program will now load twice as fast.
