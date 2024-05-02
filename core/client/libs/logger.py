import logging

class CustomFormatter(logging.Formatter):

    grey = "\x1b[38;20m"
    yellow = "\x1b[33;20m"
    green = "\x1b[32;20m"
    blue = "\x1b[34;20m"
    red = "\x1b[31;20m"
    bold_red = "\x1b[31;1m"
    reset = "\x1b[0m"
    time = "[%(asctime)s] "
    header = " %(levelname)s: "
    message = "%(message)s"
    bottom = " (%(filename)s:%(lineno)d)"

    FORMATS = {
        logging.DEBUG: time + blue + header + reset + message + blue + bottom + reset,
        logging.INFO: time + green + header + reset  + message + green + bottom + reset,
        logging.WARNING: time + yellow + header + reset  + message + yellow + bottom + reset,
        logging.ERROR: time + red + header + reset  + message + red + bottom + reset,
        logging.CRITICAL: time + bold_red + header + reset  + message + bold_red + bottom + reset
    }

    def format(self, record):
        log_fmt = self.FORMATS.get(record.levelno)
        formatter = logging.Formatter(log_fmt)
        return formatter.format(record)

log = logging.getLogger("FDESolverServerLogger")
log.handlers.clear()
log.setLevel(logging.DEBUG)
ch = logging.StreamHandler()
ch.setLevel(logging.DEBUG)
ch.setFormatter(CustomFormatter())
log.addHandler(ch)