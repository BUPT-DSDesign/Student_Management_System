import { service } from '@/utils/request'

export const IsCourseArrive = (curTime) => {
    const url = '/polling/is_course_arrive?cur_time=' + curTime
    return service({
        url: url,
        method: 'get',
    })
}
export const IsEventArrive = (curTime) => {
    const url = '/polling/is_event_arrive?cur_time=' + curTime
    return service({
        url: url,
        method: 'get',
    })
}