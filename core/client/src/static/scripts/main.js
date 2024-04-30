$(
    function() {
        $('#run_task', document).on("click", function(){

            let config = $('#config_textarea').val();
            console.log("Task running");
            window.location.href = "http://localhost:8002/" + btoa(config.replace(/\t|\v|\s|\n/g, ""));
        });
    }
);
