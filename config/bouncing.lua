this.register_on_bounce(function(axis, id, _)
    dvd.change_logo_by_id(id)
    dvd.bounce_by_id(id,axis)
end)
