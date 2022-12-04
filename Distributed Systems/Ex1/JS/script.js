// This is a jQuery method that works after the page has been loaded (ready)
$(document).ready(function() {

    // variables for processing the date strings in order to use them for the wikimedia API
    let serverCurrDate;
    let serverCurrDateForWiki;
    let lastWeekDateForWiki;
    let tempDateFormate;
    let tempDateFormatArray;

    // ajax call for getting the date from the server
    $.ajax({
        url: "PHP/get_current_date.php",
        success: function(data, status, xhr) {

            // displaying the date on the web page
            $("#date_container").html(data);


            // making the needed string processing on the date preparing for the wikimedia API calls 
            serverCurrDate = data;

            tempDateFormatArray = data.split('/');

            serverCurrDateForWiki = `${tempDateFormatArray[2]}${tempDateFormatArray[1]}${tempDateFormatArray[0]}`;

            tempDateFormate = `${tempDateFormatArray[1]}/${tempDateFormatArray[0]}/${tempDateFormatArray[2]}`;

            tempDateFormate = new Date(tempDateFormate);

            tempDateFormate = new Date(tempDateFormate.getFullYear(), tempDateFormate.getMonth(), tempDateFormate.getDate() - 7);

            lastWeekDateForWiki = `${tempDateFormate.getFullYear()}${("0" + (tempDateFormate.getMonth() + 1)).slice(-2)}${("0" + tempDateFormate.getDate()).slice(-2)}`;
        },
        // dealing with the cases of erorr of the ajax calls
        error: function(request, status, error) {

            showErrorMessage("Error: Failed To Get The Date");
            console.log("Error: Failed To Get The Date");
        }
    });


    $(".num_button").click(function() {
        // ajax call for getting the images and the animal information from the zoo-animal-api
        $.ajax({
            url: `https://zoo-animal-api.herokuapp.com/animals/rand/${$(this).val()}`,
            success: function(data, status, xhr) {

                // generating animals cards [HTMl]
                animalsCardsHTML = generateAnimalsCards(data);
                $("#animals_cards_container").html(animalsCardsHTML);

                // listening for the clicks on the animals images and names to make the needed action which is to show more information about the animal
                $(".animal_image, .animal_name").click(function() {

                    // getting the animal id in the html for showing the correct more info
                    let idParts = $(this).attr('id').split('_');

                    let idStr = idParts[idParts.length - 1];

                    let curr_more_info_block = $(`#more_info_${idStr}`);

                    if (curr_more_info_block.css('display') === 'none') {

                        let animalName = $(`#name_${idStr}`).text();

                        let animalImageViewsUrl = `https://wikimedia.org/api/rest_v1/metrics/pageviews/per-article/en.wikipedia/all-access/all-agents/${animalName}/daily/${lastWeekDateForWiki}/${serverCurrDateForWiki}`;

                        let viewsCounter = 0;

                        // calling t a method that makes ajax call for getting the number of views of the animal page
                        getAnimalImageViewsAjax(animalImageViewsUrl).then(response => {

                            response.items.forEach(element => {

                                viewsCounter += element.views;
                            });

                            $(`#views_${idStr}`).html(`${viewsCounter}`);
                        });

                        curr_more_info_block.css('display', 'block');

                    } else {

                        curr_more_info_block.css('display', 'none');
                    }


                });
            },
            // dealing with the cases of erorr of the ajax calls
            error: function(request, status, error) {

                showErrorMessage("Error: Failed To Get The Images");
                console.log("Error: Failed To Get The Date");
            }
        });
    });

    $(".error_message_button").click(function() {
        $(".error_full_cover").css('display', 'none');
    });
});

// This function gets the animals image views number 
function getAnimalImageViewsAjax(animalImageViewsUrl) {

    return $.ajax({
        url: animalImageViewsUrl,
        error: function(request, status, error) {
            // showErrorMessage("Error: Failed To Get The Views Number");
            console.log("Error: Failed To Get The Views Number");
        }

    });
}

// This function shows an error message in case of error
function showErrorMessage(errorMessage) {

    $(".error_message_span").text(errorMessage);
    $(".error_full_cover").css('display', 'block');
}

//This function generates the html animal cards with special ID for each animal card and items
function generateAnimalsCards(data) {

    let animalsCardsHTML = "";

    data.forEach((element, index) => {
        animalsCardsHTML +=
            `<div class="animal_card">
            <div class="animal_image_name_container">
                <img id="img_${index}" class="animal_image" src="${element.image_link}"/>
                <span id="name_${index}" class="animal_name">${element.name}</span>
            </div>
        <div id="more_info_${index}" class="animal_more_info">
            <div class="animal_more_info_block">
                <span class="more_info_title_span">Family: </span>
                <span class="more_info_content_span">${element.animal_type}</span>
            </div>

            <div class="animal_more_info_block">
                <span class="more_info_title_span">Food: </span>
                <span class="more_info_content_span">${element.diet}</span>
            </div>

            <div class="animal_more_info_block">
                <span class="more_info_title_span">Life Span: </span>
                <span class="more_info_content_span">${element.lifespan}</span>
            </div>

            <div class="animal_more_info_block">
                <span class="more_info_title_span">Max Length: </span>
                <span class="more_info_content_span">${(parseFloat(element.length_max) * 0.3048).toFixed(2)} m</span>
            </div>

            <div class="animal_more_info_block">
                <span class="more_info_title_span">Min Length: </span>
                <span class="more_info_content_span">${(parseFloat(element.length_min) * 0.3048).toFixed(2)} m</span>
            </div>

            <div class="animal_more_info_block">
                <span class="more_info_title_span">Max Weight: </span>
                <span class="more_info_content_span">${(parseFloat(element.weight_max) * 0.453592).toFixed(2)} kg</span>
            </div>

            <div class="animal_more_info_block">
                <span class="more_info_title_span">Min Weight: </span>
                <span class="more_info_content_span">${(parseFloat(element.weight_min) * 0.453592).toFixed(2)} kg</span>
            </div>

            <div class="animal_more_info_block">
                <span class="more_info_title_span">Views: </span>
                <span id="views_${index}" class="more_info_content_span"></span>
            </div>
        </div>
    </div>`;
    });

    return animalsCardsHTML;
}