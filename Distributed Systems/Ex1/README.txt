== Student Details ==

Student Name: Muath Abu Jamal

== Web Site URL ==

http://muathab.mysoft.jce.ac.il/Ex1/

== Question 5 ==

1-) a-) Three handshake TCP connection process.
	b-) Client sends HTTP/S request to the web server [HTTP/S Request].
	c-) The web server replies with HTTP/S response for the client request [HTTP/S Response].

	In this exercise we use the HTTP/S protocol for the communication between the user
	and the web server, and all the HTTP/S requests I make using the ajax Technique.

2-) I will compare/explain two get animal-image views-number requests:
	
	a-) Failed request:

		General:

			Request Method: GET => Asking the server to give us data.

			Status Code: 404 => The url that we are searching for not found.

		Response Headers:

			access-control-allow-headers: accept, content-type, content-length, cache-control, accept-language, api-user-agent, if-match, if-modified-since, if-none-match, dnt, accept-encoding => indicates which HTTP headers can be used during the request.

			access-control-allow-methods: GET,HEAD => indicates which HTTP Methods can be used during the request.

			access-control-allow-origin: * => the "*" tells browsers to allow requesting code from any origin to access the resource.
			
			content-length: 468 => the response content length.

			ontent-type: application/problem+json => indecates what is the response content data type. 


	b-) Succeeded request:


		General:

			Request Method: GET => Asking the server to give us data.

			Status Code: 200 => The url that we are searching for exists and we got a response without any problem [Ok].

		Response Headers:

			access-control-allow-headers: accept, content-type, content-length, cache-control, accept-language, api-user-agent, if-match, if-modified-since, if-none-match, dnt, accept-encoding => indicates which HTTP headers can be used during the request.

			access-control-allow-methods: GET,HEAD => indicates which HTTP Methods can be used during the request.

			access-control-allow-origin: * => the "*" tells browsers to allow requesting code from any origin to access the resource.
			
			content-length: 188 => the response content length.

			content-type: application/problem+json => indecates what is the response content data type.


	In this web application we got almost the same headers for all the request even if they are to get the animals information, 
	or to get the views number, but in other web sites that use deffrent APIs we can get differences for example in the CORS Headers
	because some APIs allow just certian domains to contact with them so the value of the header "access-control-allow-origin:"
	will not be "*" it will be the domains names for the allowed domains that can talk with the API.


== Program Name & Discription ==

Program Name: Animals - Web Page.

Program Description: This web application is a page that shows randumaly animals information, the animals number choosen by the user [4, 8, 10].


== Ajax Calls ==

1. Ajax call for getting the date from the server using a given PHP script.

2. Ajax call for getting the animals information.

3. Ajax call for getting the animal image-views number.


== My/Private Functions ==

1. function generateAnimalsCards(data) - This function generates the html animal cards with special ID for each animal card and items.

2. function getAnimalImageViewsAjax(animalImageViewsUrl) - This function gets the animals image views number.

3. function showErrorMessage(errorMessage) - This function shows an error message in case of error.


== Program Files ==

1. index.html - This file contains the HTML code for the web page.

2. CSS/style.css - This file contains the CSS-Style for the web page.

3. JS/script.js - This file contains the JavaScript code for the web page which contains the ajax calls.

4. PHP/get_current_date.php - This file contains a PHP script that gets the date from the server. 

5. README.txt - This file contains the student and program details.

