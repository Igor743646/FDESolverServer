$(
    function() {
        $('#run_task', document).on("click", function(){

            let config = $('#config_textarea').text();
        
            console.log("Task running");
            console.log(config);
        });
    }
);
