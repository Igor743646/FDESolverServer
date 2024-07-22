$("body", document).ready(function() {

    $('#file-input').change(function () {
        var $form = $(this);
        var file = $form[0].files[0];
        var type = file.type

        console.log(file.name, type)

        if (!(type === "text/plain" || type === "text/json")) {
            console.error("Can not read file " + file.name + " with type: " + type)
            return
        }


        const reader = new FileReader()
        reader.onload = () => {
            $('#config_textarea').val(reader.result)
        };  
        reader.readAsText(file)
        
    });
    
});
