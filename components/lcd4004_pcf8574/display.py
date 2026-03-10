import esphome.codegen as cg
from esphome.components import i2c, lcd_base
import esphome.config_validation as cv
from esphome.const import CONF_DIMENSIONS, CONF_ID, CONF_LAMBDA

DEPENDENCIES = ["i2c"]
AUTO_LOAD = ["lcd_base"]

lcd_pcf8574_ns = cg.esphome_ns.namespace("lcd4004_pcf8574")
PCF8574LCDDisplay = lcd_pcf8574_ns.class_(
    "PCF8574LCDDisplay", lcd_base.LCDDisplay, i2c.I2CDevice
)

def validate_40x4_dimensions(config):
    if tuple(config[CONF_DIMENSIONS]) != (40, 4):
        raise cv.Invalid("lcd4004_pcf8574 requires dimensions: 40x4")
    return config


CONFIG_SCHEMA = cv.All(
    lcd_base.LCD_SCHEMA.extend(
        {
            cv.GenerateID(): cv.declare_id(PCF8574LCDDisplay),
        }
    ).extend(i2c.i2c_device_schema(0x3F)),
    validate_40x4_dimensions,
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await lcd_base.setup_lcd_display(var, config)
    await i2c.register_i2c_device(var, config)

    if CONF_LAMBDA in config:
        lambda_ = await cg.process_lambda(
            config[CONF_LAMBDA],
            [(PCF8574LCDDisplay.operator("ref"), "it")],
            return_type=cg.void,
        )
        cg.add(var.set_writer(lambda_))
