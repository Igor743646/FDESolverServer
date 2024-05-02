$(
    function() {
        $('#run_task', document).on("click", function(){
            
            let config = $('#config_textarea').val();
            let configCoded = btoa(encodeURIComponent(config.replace(/\t|\v|\s|\n/g, "")));
            window.location.href = "http://localhost:8002/" + configCoded;
        });
    }
);
