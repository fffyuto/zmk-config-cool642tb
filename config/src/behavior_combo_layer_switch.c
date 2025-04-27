#include <zephyr/device.h>
#include <zephyr/drivers/behavior.h>
#include <zephyr/kernel.h>
#include <zmk/behavior.h>
#include <zmk/event_manager.h>
#include <zmk/events/keycode_state_changed.h>
#include <zmk/events/layer_state_changed.h>
#include <zmk/layers.h>

struct behavior_combo_layer_switch_config {
    uint8_t layer;
};

static int on_press(const struct device *dev, struct zmk_behavior_binding_event event) {
    const struct behavior_combo_layer_switch_config *cfg = dev->config;
    zmk_layer_activate(cfg->layer);
    return 0;
}

static int on_release(const struct device *dev, struct zmk_behavior_binding_event event) {
    const struct behavior_combo_layer_switch_config *cfg = dev->config;
    zmk_layer_deactivate(cfg->layer);
    return 0;
}

static const struct behavior_driver_api behavior_combo_layer_switch_driver_api = {
    .binding_pressed = on_press,
    .binding_released = on_release,
};

#define DT_DRV_COMPAT zmk_behavior_combo_layer_switch

#include <zephyr/device.h>
#include <zephyr/init.h>
#include <zephyr/sys/util_macro.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/behavior.h>

#define DEFINE_BEHAVIOR(inst)                                           \
    static const struct behavior_combo_layer_switch_config behavior_combo_layer_switch_config_##inst = { \
        .layer = DT_INST_PROP(inst, layer),                             \
    };                                                                  \
    DEVICE_DT_INST_DEFINE(inst, NULL, NULL,                             \
                          NULL, &behavior_combo_layer_switch_config_##inst, \
                          APPLICATION, CONFIG_KERNEL_INIT_PRIORITY_DEFAULT, \
                          &behavior_combo_layer_switch_driver_api);

DT_INST_FOREACH_STATUS_OKAY(DEFINE_BEHAVIOR)
