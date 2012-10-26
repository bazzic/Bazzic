<?php


class crumbs_InvokeAction_decorateBreadcrumb implements crumbs_InvokeActionInterface_alter {

  protected $breadcrumb;

  function __construct(array &$breadcrumb) {
    $this->breadcrumb = &$breadcrumb;
  }

  function invoke($plugin, $plugin_key) {
    if (method_exists($plugin, 'decorateBreadcrumb')) {
      $plugin->decorateBreadcrumb($this->breadcrumb);
    }
  }
}
