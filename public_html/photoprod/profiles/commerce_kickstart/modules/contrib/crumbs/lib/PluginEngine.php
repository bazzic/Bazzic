<?php


class crumbs_PluginEngine {

  protected $plugins;
  protected $weightKeeper;

  protected $pluginOrder_find = array();
  protected $pluginOrder_alter = array();

  function __construct(array $plugins, array $weights) {
    $this->plugins = $plugins;
    foreach ($plugins as $plugin_key => $plugin) {
      // $weights[$plugin_key] = FALSE;
    }
    $this->weightKeeper = new crumbs_RuleWeightKeeper($weights);

    foreach ($plugins as $plugin_key => $plugin) {
      $keeper = $this->weightKeeper->prefixedWeightKeeper($plugin_key);
      $w_find = $keeper->getSmallestWeight();
      if ($w_find !== FALSE) {
        $this->pluginOrder_find[$plugin_key] = $w_find;
      }
      $w_alter = $keeper->findWeight();
      if ($w_alter !== FALSE) {
        $this->pluginOrder_alter[$plugin_key] = $w_alter;
      }
    }
    // lowest weight first = highest priority first
    asort($this->pluginOrder_find);
    // lowest weight last = highest priority last
    arsort($this->pluginOrder_alter);

    foreach ($this->pluginOrder_find as $plugin_key => $weight) {
      $this->pluginOrder_find[$plugin_key] = $plugins[$plugin_key];
    }
    foreach ($this->pluginOrder_alter as $plugin_key => $weight) {
      $this->pluginOrder_alter[$plugin_key] = $plugins[$plugin_key];
    }
  }

  /**
   * Invoke the invoke action for all plugins, starting with the plugin with
   * highest priority. The function will stop when it has 
   *
   * @param $invokeAction
   *   an object that does the method call, and can maintain a state between
   *   different plugins' method calls.
   */
  function invokeAll_find(crumbs_InvokeActionInterface_find $invoke_action) {
    foreach ($this->pluginOrder_find as $plugin_key => $plugin) {
      $weight_keeper = $this->weightKeeper->prefixedWeightKeeper($plugin_key);
      $found = $invoke_action->invoke($plugin, $plugin_key, $weight_keeper);
      if ($found) {
        return $found;
      }
    }
  }

  /**
   * invokeAll for alter hooks.
   * These need to be called with the lowest priority first,
   * because later calls will overwrite earlier calls.
   */
  function invokeAll_alter(crumbs_InvokeActionInterface_alter $invokeAction) {
    foreach ($this->pluginOrder_alter as $plugin_key => $plugin) {
      $invokeAction->invoke($plugin, $plugin_key);
    }
  }
}
