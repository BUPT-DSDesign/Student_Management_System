import { defineStore } from 'pinia'
import { IsCourseArrive, IsEventArrive } from '@/api/polling'
import { ref } from 'vue'

export const usePollingStore = defineStore('polling', () => {
    const pollingCourse = ref({});
    const pollingEvent = ref({});
    const is_course_arrive = ref(false);
    const is_event_arrive = ref(false);
    const GetArrivedCourse = async (curTime) => {
        try {
            const res = await IsCourseArrive(curTime);
            console.log(res.data)
            if (res.data.status_code == 0) {
                pollingCourse.value = res.data.course_info;
                is_course_arrive.value = res.data.is_arrive;
                return true
            }
            console.log(res.data)
            return false
        } catch (err) {
            return false
        }
    }
    const GetArrivedEvent = async (curTime) => {
        try {
            const res = await IsEventArrive(curTime);
            if (res.data.status_code == 0) {
                pollingEvent.value = res.data.activity_info;
                is_event_arrive.value = res.data.is_arrive;
                return true
            }
            console.log(res.data)
            return false
        } catch (err) {
            return false
        }
    }
    return {
        pollingCourse,
        pollingEvent,
        is_event_arrive,
        is_course_arrive,
        GetArrivedCourse,
        GetArrivedEvent
    }
})
