package com.example.oh_sungjin.prjarachne_smartmirror.Widget.Calendar;

import android.content.Context;
import android.graphics.Color;
import android.view.View;

import com.example.oh_sungjin.prjarachne_smartmirror.R;
import com.prolificinteractive.materialcalendarview.CalendarDay;
import com.prolificinteractive.materialcalendarview.CalendarMode;
import com.prolificinteractive.materialcalendarview.MaterialCalendarView;

import java.util.Calendar;

public class MyCalendar extends View {
    private MaterialCalendarView calView;

    public MyCalendar(Context context) {
        super(context);
        this.calView = new MaterialCalendarView(context);

        this.calView.state().edit()
                .setFirstDayOfWeek(Calendar.SUNDAY)
                .setMinimumDate(CalendarDay.from(2018, 0, 1))
                .setMaximumDate(CalendarDay.from(2030, 11, 31))
                .setCalendarDisplayMode(CalendarMode.MONTHS)
                .commit();

        this.calView.addDecorators(
                new DayDecorator(),
                new SundayDecorator(),
                new SaturdayDecorator(),
                new OneDayDecorator());

        this.calView.setHeaderTextAppearance(R.style.CalendarWidgetHeader);
        this.calView.setWeekDayTextAppearance(R.style.CalendarWidgetWeekDay);

        this.calView.setTileHeight(23);

        this.calView.setSelectionMode(MaterialCalendarView.SELECTION_MODE_NONE);

    }

    public MaterialCalendarView getCalView() {
        return this.calView;
    }

}
